#include "Board.h"
#include "Player.h"
#include "TownHall.h"
#include "Entities/Troops/Archer.h"
#include "Entities/Troops/Barbarian.h"
#include "Entities/Troops/Troop.h"
#include "Entities/Enemies/Enemy.h"
#include "Entities/Enemies/Raider.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <typeinfo>

Board::Board(int sX, int sY) : SizeX(sX), SizeY(sY) {}

Board::~Board() {
    for (auto* b : buildings) delete b;
    for (auto* e : entities) delete e;
    for (auto* t : troops) delete t;
    for (auto* e : enemies) delete e;
}

bool Board::IsOutOfBounds(Position pos) const {
    return pos.x < 0 || pos.x >= SizeX || pos.y < 0 || pos.y >= SizeY;
}

bool Board::CollidesWith(const Building& nb) const {
    for (const Building* b : buildings)
        if (b->collidesWith(nb)) return true;
    return false;
}

int Board::CountBuildingType(const Building& target) const {
    int count = 0;
    for (const Building* b : buildings)
        if (typeid(*b) == typeid(target)) count++;
    return count;
}

bool Board::AddBuilding(Building* nb) {
    if (IsOutOfBounds(nb->getPosition())) return false;
    if (CollidesWith(*nb)) return false;
    if (nb->getMaxInstances() <= CountBuildingType(*nb)) { delete nb; return false; }
    if (player && !player->getResources().canAfford(nb->getCost())) return false;
    if (player) player->getResources().consume(nb->getCost());
    buildings.push_back(nb);
    if (auto* th = dynamic_cast<TownHall*>(nb)) townHall = th;
    return true;
}

bool Board::isPositionValid(const Position& pos) const {
    if (pos.x < 0 || pos.x >= SizeX || pos.y < 0 || pos.y >= SizeY) return false;
    for (const Building* b : buildings)
        if (b->collidesWith(pos)) return false;
    return true;
}

void Board::removeBuilding(Building* b) {
    auto it = std::find(buildings.begin(), buildings.end(), b);
    if (it != buildings.end()) { buildings.erase(it); delete b; }
}

void Board::addEntity(Entity* e) { entities.push_back(e); }

std::vector<Entity*>& Board::getEntities() { return entities; }

std::vector<std::string> Board::getRenderLines() const {
    std::vector<std::vector<std::string>> grid(SizeY, std::vector<std::string>(SizeX, "  "));

    for (const Building* b : buildings) {
        int cx = b->getPosition().x, cy = b->getPosition().y;
        int hw = b->getSizeX() / 2, hh = b->getSizeY() / 2;
        for (int r = cy - hh; r <= cy + hh; r++) {
            for (int c = cx - hw; c <= cx + hw; c++) {
                if (r >= 0 && r < SizeY && c >= 0 && c < SizeX) {
                    if (r == cy && c == cx) {
                        grid[r][c] = b->getRepr();
                    } else if (r == cy - hh || r == cy + hh || c == cx - hw || c == cx + hw) {
                        grid[r][c] = "🟫";
                    }
                }
            }
        }
    }

    for (const Entity* e : entities) {
        int x = e->getPosition().x, y = e->getPosition().y;
        if (x >= 0 && x < SizeX && y >= 0 && y < SizeY)
            grid[y][x] = e->getRepr();
    }

    for (const Enemy* e : enemies) {
        int x = e->getPosition().x, y = e->getPosition().y;
        if (x >= 0 && x < SizeX && y >= 0 && y < SizeY)
            grid[y][x] = e->getRepr();
    }

    for (const Troop* t : troops) {
        if (t->getState() != TroopState::IDLE) {
            int x = t->getPosition().x, y = t->getPosition().y;
            if (x >= 0 && x < SizeX && y >= 0 && y < SizeY)
                grid[y][x] = t->getRepr();
        }
    }

    if (player) {
        int x = player->getPosition().x, y = player->getPosition().y;
        if (x >= 0 && x < SizeX && y >= 0 && y < SizeY)
            grid[y][x] = player->getRepr();
    }

    std::vector<std::string> lines;
    for (int row = 0; row < SizeY; row++) {
        std::string line = "|";
        for (int col = 0; col < SizeX; col++) line += grid[row][col];
        line += "|";
        lines.push_back(line);
    }
    return lines;
}

void Board::addTroop(Troop* t) {
    if (townHall) {
        t->setHome(townHall->getPosition());
        t->setState(TroopState::RETURNING);
    }
    troops.push_back(t);
}

void Board::addEnemy(Enemy* e) { enemies.push_back(e); }

void Board::updateTroops() {
    for (Troop* t : troops) t->Update(*this);
}

void Board::updateEnemies() {
    for (Enemy* e : enemies) e->Update(*this);
}

void Board::cleanDead() {
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        Enemy* e = *it;
        if (!e->isAlive()) {
            if (e->getAssignedTroop()) e->getAssignedTroop()->setTarget(nullptr);
            Troop* killer = e->getLastAttacker();
            if (dynamic_cast<Barbarian*>(killer)) {
                score += 100;
            } else if (dynamic_cast<Archer*>(killer)) {
                score += 50;
            }
            if (dynamic_cast<Raider*>(e)) ++raidersKilled;
            delete e;
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = troops.begin(); it != troops.end(); ) {
        Troop* t = *it;
        if (!t->isAlive()) {
            if (t->getTarget()) t->getTarget()->setAssignedTroop(nullptr);
            delete t;
            it = troops.erase(it);
        } else {
            ++it;
        }
    }
}

void Board::tickRaiderSpawn() {
    if (!townHall) return;
    int spawnInterval = std::max(25, 100 - elapsedTicks / 20);
    int spawnCount = std::min(3, 1 + elapsedTicks / 300);
    if (--raiderTimer > 0) return;
    raiderTimer = spawnInterval;

    for (int i = 0; i < spawnCount; ++i) {
        int edge = rand() % 4;
        int rx, ry;
        switch (edge) {
            case 0: rx = rand() % SizeX; ry = 0; break;
            case 1: rx = rand() % SizeX; ry = SizeY - 1; break;
            case 2: rx = 0; ry = rand() % SizeY; break;
            default: rx = SizeX - 1; ry = rand() % SizeY; break;
        }
        enemies.push_back(new Raider(Position(rx, ry)));
    }
}