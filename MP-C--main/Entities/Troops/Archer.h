#pragma once
#include "Troop.h"

class Archer : public Troop {
public:
    Archer(Position pos);
    int getRange() const override { return 6; } // long ranged
};