#include "ElixirCollector.h"
// SizeX=3 SizeY=3 Cost=100 gold/0 elixir MaxInstances=3 HP=150
ElixirCollector::ElixirCollector(Position p)
    : ResourceGenerator(p, 3, 3,"🧪", Resources(100,0), 3, 150) {}

Resources ElixirCollector::Collect() {
    int n = current; current = 0; isFull = false; repr = "🧪";
    return Resources(0, n);
}
void ElixirCollector::update(Board& b) {
    ResourceGenerator::update(b);
    repr = isFull ? "💧" :"🧪";
}