#include "Player.h"
#include "Board.h"
#include "GoldMine.h"
#include "ElixirCollector.h"

Player::Player(Position pos)
    : Entity(pos, "🧑"), resources(600, 600) {}   
Player::~Player() {}

Building* Player::Interact(Board& board) {
    for (Building* b : board.getBuildings()) {
        if (b->collidesWith(position)) {
            if (auto* m = dynamic_cast<GoldMine*>(b))
                resources.addGold(m->Collect().getGold());
            else if (auto* e = dynamic_cast<ElixirCollector*>(b))
                resources.addElixir(e->Collect().getElixir());
        }
    }
    return nullptr;
}

void Player::moving(const Board& board, int dx, int dy) {
    Position newPos = position + Position(dx, dy);
    // Only check map boundaries, not buildings
    if (newPos.x < 0 || newPos.y < 0 ||
        newPos.x >= board.getSizeX() || newPos.y >= board.getSizeY()) return;
    position = newPos;
}

void Player::Update(Board& /*b*/) {}

Resources&       Player::getResources()       { return resources; }
const Resources& Player::getResources() const { return resources; }