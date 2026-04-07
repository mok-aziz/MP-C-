#include "TownHall.h"
// SizeX=5 SizeY=5 Cost=100/0 MaxInstances=1 HP=1000
TownHall::TownHall(Position p)
   : Building(p, 5, 5, "🏰", Resources(100, 0), 1, 1000) {}