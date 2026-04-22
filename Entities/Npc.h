#ifndef NPC_H
#define NPC_H
#include "Entity.h"

class Board;

class Npc : public Entity {
protected:
    int health;
public:
    Npc(Position pos, int hp = 100, const std::string& repr = "?");
    virtual ~Npc();
    void loseHealth(int amount);
    void moveTowards(const Board& board, Position dest);
    int  getHealth() const { return health; }
    virtual void Update(Board& board) = 0;
};
#endif