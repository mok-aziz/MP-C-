#include "Raider.h"

Raider::Raider(Position pos)
    : Enemy(50, 10, "👺") {
    position = pos;
}