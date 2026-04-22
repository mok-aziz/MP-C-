#include "Troop.h"
#include "../../Board.h"
#include "../Enemies/Enemy.h"
#include <cmath>
#include <algorithm>
#include <vector>

Troop::Troop(Position pos, int hp, int dmg, const std::string& repr, int maxKills)
    : Npc(pos, hp, repr), attackDamage(dmg), maxEnemyKills(maxKills) {
    position = pos;
    homePos = pos;
}

void Troop::Update(Board& board) {
    int speedTier = board.getElapsedTicks() / 200;
    int currentCooldown = std::max(1, 3 - speedTier);
    if (moveCooldown > 0) { --moveCooldown; return; }
    moveCooldown = currentCooldown;

    auto& enemies = board.getEnemies();

    // ── IDLE: look for an unengaged enemy ─────────────────────────────
    if (state == TroopState::IDLE) {
        for (Enemy* e : enemies) {
            if (e && e->isAlive() && !e->hasAssignedTroop()) {
                target = e;
                e->setAssignedTroop(this);
                state = TroopState::FIGHTING;
                break;
            }
        }
        return;
    }

    // ── FIGHTING ──────────────────────────────────────────────────────
    if (state == TroopState::FIGHTING) {
        if (!target || !target->isAlive()) {
            if (target) target->setAssignedTroop(nullptr);
            target = nullptr;
            state = TroopState::RETURNING;
            return;
        }

        int dx = target->getPosition().x - position.x;
        int dy = target->getPosition().y - position.y;
        int dist = std::abs(dx) + std::abs(dy);

        if (dist <= getRange()) {
            // In range — deal damage (kills enemy on hit for Barbarian, takes a few for Archer)
            target->setLastAttacker(this);
            target->takeDamage(attackDamage);
            if (!target->isAlive()) {
                ++enemyKills;
                target = nullptr;
                if (enemyKills >= maxEnemyKills) {
                    health = 0;
                    return;
                }
                state = TroopState::RETURNING;
            }
        } else {
            // Move toward enemy
            if (std::abs(dx) >= std::abs(dy))
                position.x += (dx > 0) ? 1 : -1;
            else
                position.y += (dy > 0) ? 1 : -1;
        }
        return;
    }

    // ── RETURNING ─────────────────────────────────────────────────────
    if (state == TroopState::RETURNING) {
        int dx = homePos.x - position.x;
        int dy = homePos.y - position.y;
        if (dx == 0 && dy == 0) {
            state = TroopState::IDLE;
            return;
        }
        if (std::abs(dx) >= std::abs(dy))
            position.x += (dx > 0) ? 1 : -1;
        else
            position.y += (dy > 0) ? 1 : -1;
    }
}