#include "ResourceGenerator.h"

ResourceGenerator::ResourceGenerator(Position p, int sX, int sY,
    const std::string& r, Resources cost, int maxInst, int hp)
    : Building(p, sX, sY, r, cost, maxInst, hp),
      current(0), capacity(100), isFull(false),
      lastProductionTime(std::chrono::steady_clock::now()),
      productionCooldownMs(3000) {}

ResourceGenerator::~ResourceGenerator() {}

void ResourceGenerator::update(Board& /*b*/) {
    auto now = std::chrono::steady_clock::now();
    int ms = (int)std::chrono::duration_cast<std::chrono::milliseconds>
             (now - lastProductionTime).count();
    if (!isFull && ms >= productionCooldownMs) {
        current += 10;
        if (current >= capacity) { current = capacity; isFull = true; }
        lastProductionTime = now;
    }
}