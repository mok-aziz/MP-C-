#pragma once
#include "Npc.h"
#include "Buildings/Building.h"

class Board;
class Troop;

class Enemy : public Npc {
protected:
    int attackDamage;
    int moveCooldown = 0;
    Troop* assignedTroop = nullptr;
    Troop* lastAttacker = nullptr;

public:
    Enemy(int hp, int dmg, const std::string& repr);
    virtual ~Enemy() = default;

    virtual void Update(Board& board) override;

    bool isAlive() const { return health > 0; }
    bool hasAssignedTroop() const { return assignedTroop != nullptr; }
    void setAssignedTroop(Troop* t) { assignedTroop = t; }
    Troop* getAssignedTroop() const { return assignedTroop; }
    void setLastAttacker(Troop* t) { lastAttacker = t; }
    Troop* getLastAttacker() const { return lastAttacker; }
};