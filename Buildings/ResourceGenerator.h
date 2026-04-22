#ifndef RESOURCEGENERATOR_H
#define RESOURCEGENERATOR_H
#include "Building.h"
#include <chrono>

class ResourceGenerator : public Building {
protected:
    int  current;
    int  capacity;       // 100 par defaut
    bool isFull;
    std::chrono::steady_clock::time_point lastProductionTime;
    int  productionCooldownMs;
public:
    ResourceGenerator(Position p, int sX, int sY,
                      const std::string& repr,
                      Resources cost, int maxInst, int hp);
    virtual ~ResourceGenerator();
    virtual void update(Board& board) override;
    bool full()        const { return isFull; }
    int  getCurrent()  const { return current; }
    int  getCapacity() const { return capacity; }
};
#endif