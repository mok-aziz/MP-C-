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
    void takeDamage(int dmg) { health -= dmg; if (health < 0) health = 0; }
    bool isAlive() const { return health > 0; }
};
#endif