#ifndef BOARD_H
#define BOARD_H
#include "Position.h"
#include "Building.h"
#include "TownHall.h"
#include "Entity.h"
#include <vector>
#include <string>

class Player;

class Board {
private:
    int SizeX, SizeY;
    std::vector<Building*> buildings;
    std::vector<Entity*>   entities;
    TownHall* townHall = nullptr;
    Player*   player   = nullptr;

public:
    Board(int sizeX, int sizeY);
    virtual ~Board();

    bool IsOutOfBounds(Position pos) const;
    bool isPositionValid(const Position& pos) const;
    bool CollidesWith(const Building& b) const;
    bool AddBuilding(Building* b);
    void removeBuilding(Building* b);
    int  CountBuildingType(const Building& target) const;

    void addEntity(Entity* e);
    std::vector<Entity*>& getEntities();

    // Returns each row as a string (for side-by-side console render)
    std::vector<std::string> getRenderLines() const;

    int       getSizeX()    const { return SizeX; }
    int       getSizeY()    const { return SizeY; }
    const std::vector<Building*>& getBuildings() const { return buildings; }
    Player*   getPlayer()   const { return player; }
    TownHall* getTownHall() const { return townHall; }
    void setPlayer(Player* p)    { player   = p; }
    void setTownHall(TownHall* t){ townHall = t; }
};
#endif