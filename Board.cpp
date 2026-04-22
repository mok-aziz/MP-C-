#include "Board.h"
#include "Player.h"
#include "TownHall.h"
#include <algorithm>
#include <typeinfo>
#include <iostream>

Board::Board(int sX, int sY) : SizeX(sX), SizeY(sY) {}
Board::~Board() {
    for (auto* b : buildings) delete b;
    for (auto* e : entities)  delete e;
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
    if (IsOutOfBounds(nb->getPosition()))              return false;
    if (CollidesWith(*nb))                             return false;
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
    // Use 2-char wide cells to match emoji display width
    std::vector<std::vector<std::string>> grid(SizeY,
        std::vector<std::string>(SizeX, ". "));

    for (const Building* b : buildings) {
        int cx = b->getPosition().x, cy = b->getPosition().y;
        int hw = b->getSizeX()/2, hh = b->getSizeY()/2;
        for (int r = cy-hh; r <= cy+hh; r++)
            for (int c = cx-hw; c <= cx+hw; c++)
                if (r >= 0 && r < SizeY && c >= 0 && c < SizeX)
                    grid[r][c] = (r==cy && c==cx) ? b->getRepr() : "🟫";
    }
    for (const Entity* e : entities) {
        int x = e->getPosition().x, y = e->getPosition().y;
        if (x >= 0 && x < SizeX && y >= 0 && y < SizeY)
            grid[y][x] = e->getRepr();
    }
    if (player) {
        int x = player->getPosition().x, y = player->getPosition().y;
        if (x >= 0 && x < SizeX && y >= 0 && y < SizeY)
            grid[y][x] = player->getRepr();
    }

    std::vector<std::string> lines;
    for (int row = 0; row < SizeY; row++) {
        std::string line = "|";
        for (int col = 0; col < SizeX; col++)
            line += grid[row][col];
        line += "|";
        lines.push_back(line);
    }
    return lines;
}