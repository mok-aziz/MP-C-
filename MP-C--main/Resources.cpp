#include "Resources.h"

Resources::Resources(int g, int e) : gold(g), elixir(e) {}

void Resources::addGold(int a)   { gold   += a; }
void Resources::addElixir(int a) { elixir += a; }

bool Resources::spendGold(int a) {
    if (gold >= a) { gold -= a; return true; } return false;
}
bool Resources::spendElixir(int a) {
    if (elixir >= a) { elixir -= a; return true; } return false;
}

int  Resources::getGold()   const { return gold; }
int  Resources::getElixir() const { return elixir; }

bool Resources::canAfford(const Resources& c) const {
    return gold >= c.getGold() && elixir >= c.getElixir();
}
void Resources::consume(const Resources& c) {
    gold -= c.getGold(); elixir -= c.getElixir();
}