#include "Building.h"

Building::Building(Position pos, int sX, int sY, const std::string& r,
                   Resources c, int maxInst, int hp)
    : p(pos), sizeX(sX), sizeY(sY), repr(r), cost(c),
      maxInstances(maxInst), health(hp) {}

Building::~Building() {}

bool Building::collidesWith(Position pos) const {
    return (pos.x >= p.x - sizeX/2 && pos.x <= p.x + sizeX/2 &&
            pos.y >= p.y - sizeY/2 && pos.y <= p.y + sizeY/2);
}
bool Building::collidesWith(const Building& o) const {
    return !(p.x + sizeX/2 < o.p.x - o.sizeX/2 ||
             p.x - sizeX/2 > o.p.x + o.sizeX/2 ||
             p.y + sizeY/2 < o.p.y - o.sizeY/2 ||
             p.y - sizeY/2 > o.p.y + o.sizeY/2);
}
void Building::update(Board& /*b*/) {}
void Building::loseHealth(int a) { health -= a; if (health < 0) health = 0; }