#pragma once
#include "Troop.h"

class Barbarian : public Troop {
public:
    Barbarian(Position pos);
    int getRange() const override { return 1; } // melee
};