#include "Wall.h"
// SizeX=1 SizeY=1 Cost=10 gold MaxInstances=200 HP=200
Wall::Wall(Position pos)
: Building(pos, 1, 1, "🧱", Resources(10,0), 200, 200) {}