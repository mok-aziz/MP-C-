#ifndef TOWNHALL_H
#define TOWNHALL_H
#include "Building.h"

class TownHall : public Building {
public:
    TownHall(Position p);
    virtual ~TownHall() {}
};
#endif