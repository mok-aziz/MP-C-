#pragma once
#include "Building.h"

class Board;
class Player;

class Barrack : public Building {
public:
    static const int CAPACITY = 5;

    Barrack(Position pos);
    virtual ~Barrack() = default;

    void update(Board& board) override {} // passive — no resource gen

    // Opens the console sub-menu. Blocks until 'Q' is pressed.
    void openMenu(Board& board, Player* player);
};