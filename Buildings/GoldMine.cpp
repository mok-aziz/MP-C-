#include "GoldMine.h"
// SizeX=3 SizeY=3 Cost=0 gold/100 elixir MaxInstances=3 HP=150
GoldMine::GoldMine(Position p)
    : ResourceGenerator(p, 3, 3, "⛏️", Resources(0,100), 3, 150) {}

Resources GoldMine::Collect() {
    int n = current; current = 0; isFull = false; repr = "⛏";
    return Resources(n, 0);
}
void GoldMine::update(Board& b) {
    ResourceGenerator::update(b);
   repr = isFull ? "🪙" : "⛏️"; // $ = full (coin), ⛏ = mining
}