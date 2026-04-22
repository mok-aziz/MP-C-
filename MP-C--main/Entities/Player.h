#ifndef PLAYER_H
#define PLAYER_H
#include "Entity.h"
#include "Resources.h"
#include "Building.h"

class Board;

class Player : public Entity {
private:
    Resources resources;
public:
    Player(Position pos);
    virtual ~Player();
    Building* Interact(Board& board);
    void moving(const Board& board, int dx, int dy) override;
    void Update(Board& board) override;
    Resources&       getResources();
    const Resources& getResources() const;
};
#endif