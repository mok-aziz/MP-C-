#include "Npc.h"
#include "Board.h"

Npc::Npc(Position pos, int hp, const std::string& r)
    : Entity(pos, r), health(hp) {}
Npc::~Npc() {}

void Npc::loseHealth(int a) { health -= a; if (health < 0) health = 0; }

void Npc::moveTowards(const Board& board, Position dest) {
    Position best = position;
    int bestDist  = position.manhattanDistance(dest);
    Position dirs[4] = {
        position+Position(1,0), position+Position(-1,0),
        position+Position(0,1), position+Position(0,-1)
    };
    for (auto& next : dirs) {
        if (board.isPositionValid(next)) {
            int d = next.manhattanDistance(dest);
            if (d < bestDist) { bestDist = d; best = next; }
        }
    }
    position = best;
}