#include "Entity.h"
#include "Board.h"

Entity::Entity(Position pos, const std::string& r) : position(pos), repr(r) {}
Entity::~Entity() {}

void Entity::moving(const Board& board, int dx, int dy) {
    Position newPos = position + Position(dx, dy);
    if (board.isPositionValid(newPos))
        position = newPos;
}