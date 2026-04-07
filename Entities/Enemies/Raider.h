#ifndef RAIDER_H
#define RAIDER_H

#include "Enemy.h"
#include "Building.h"
#include <chrono>
#include <vector>

class Raider : public Enemy {
private:
    std::chrono::steady_clock::time_point lastMoveTime;
    int  moveDelayMs;
    bool markedForDeletion = false;

    Building* getNearestBuilding(const std::vector<Building*>& buildings);

public:
    Raider(Position pos);
    virtual ~Raider() {}
    void Update(Board& board) override;
    bool isMarkedForDeletion() const { return markedForDeletion; }
    void markForDeletion()           { markedForDeletion = true; }
};
//test 10
#endif