#ifndef ENTITY_H
#define ENTITY_H
#include "Position.h"
#include <string>

class Board;

class Entity {
protected:
    Position    position;
    std::string repr;
public:
    Entity(Position pos, const std::string& repr);
    virtual ~Entity();
    Position    getPosition() const { return position; }
    std::string getRepr()     const { return repr; }
    virtual void moving(const Board& board, int dx, int dy);
    virtual void Update(Board& board) = 0;
    void setPosition(Position p) { position = p; }
};
#endif
