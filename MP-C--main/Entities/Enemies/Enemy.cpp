#include "Enemy.h"
#include "../../Board.h"
#include "../../Buildings/TownHall.h"
#include "../../Buildings/Wall.h"
#include <cmath>
#include <algorithm>

Enemy::Enemy(int hp, int dmg, const std::string& repr)
    : Npc(Position(), hp, repr), attackDamage(dmg) {}

void Enemy::Update(Board& board) {
    if (moveCooldown > 0) { --moveCooldown; return; }
    moveCooldown = 5;

    TownHall* th = board.getTownHall();
    if (!th) return;

    Position target = th->getPosition();
    int dx = target.x - position.x;
    int dy = target.y - position.y;

    // If already on TownHall, attack it
    if (std::abs(dx) <= th->getSizeX()/2 && std::abs(dy) <= th->getSizeY()/2) {
        th->takeDamage(attackDamage);
        return;
    }

    // Try to move, but stop and attack if a wall is in the way
    Position nextPos = position;
    if (std::abs(dx) >= std::abs(dy))
        nextPos.x += (dx > 0) ? 1 : -1;
    else
        nextPos.y += (dy > 0) ? 1 : -1;

    Building* blockingBuilding = nullptr;
    for (Building* b : board.getBuildings()) {
        if (b->collidesWith(nextPos)) {
            blockingBuilding = b;
            break;
        }
    }

    if (blockingBuilding) {
        if (dynamic_cast<Wall*>(blockingBuilding)) {
            board.removeBuilding(blockingBuilding);
        } else {
            blockingBuilding->takeDamage(attackDamage); // Attack non-wall blockers normally
        }
        return;
    }

    position = nextPos;
}