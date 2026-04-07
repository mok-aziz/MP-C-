#ifndef BUILDING_H
#define BUILDING_H
#include "Position.h"
#include "Resources.h"
#include <string>

class Board;

class Building {
protected:
    Position    p;
    int         sizeX, sizeY;
    std::string repr;
    Resources   cost;
    int         maxInstances;
    int         health;
public:
    Building(Position pos, int sX, int sY, const std::string& repr,
             Resources cost, int maxInstances, int health);
    virtual ~Building();

    bool collidesWith(Position pos) const;
    bool collidesWith(const Building& other) const;
    virtual void update(Board& board);

    Position    getPosition()    const { return p; }
    int         getSizeX()       const { return sizeX; }
    int         getSizeY()       const { return sizeY; }
    int         getHealth()      const { return health; }
    std::string getRepr()        const { return repr; }
    Resources   getCost()        const { return cost; }
    int         getMaxInstances()const { return maxInstances; }
    void        loseHealth(int amount);
};
#endif