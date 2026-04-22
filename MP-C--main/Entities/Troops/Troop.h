#pragma once
#include "../Npc.h"

class Board;
class Enemy;

enum class TroopState { IDLE, FIGHTING, RETURNING };

class Troop : public Npc {
protected:
    TroopState state = TroopState::IDLE;
    Enemy* target = nullptr;
    Position homePos;
    int moveCooldown = 0;
    int attackDamage;
    int enemyKills = 0;
    int maxEnemyKills;

public:
    Troop(Position pos, int hp, int dmg, const std::string& repr, int maxKills);
    virtual ~Troop() = default;

    virtual int getRange() const = 0;
    virtual void Update(Board& board) override;

    TroopState getState() const { return state; }
    void setState(TroopState s) { state = s; }
    void setHome(Position p) { homePos = p; }
    Position getHome() const { return homePos; }
    Enemy* getTarget() const { return target; }
    void setTarget(Enemy* e) { target = e; }
};