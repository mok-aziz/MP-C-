#ifndef BOARD_H
#define BOARD_H
#include "Position.h"
#include "Building.h"
#include "TownHall.h"
#include "Entity.h"
#include <vector>
#include <string>
#include "Entities/Troops/Troop.h"
#include "Entities/Enemies/Enemy.h"

class Player;

class Board {
private:
    int SizeX, SizeY;
    std::vector<Building*> buildings;
    std::vector<Entity*>   entities;
    TownHall* townHall = nullptr;
    Player*   player   = nullptr;
    std::vector<Troop*>  troops;
    std::vector<Enemy*>  enemies;
    int raiderTimer = 100;   // ticks until next raider spawns
    int score = 0;
    int raidersKilled = 0;
    int elapsedTicks = 0;


public:
    Board(int sizeX, int sizeY);
    virtual ~Board();

    bool IsOutOfBounds(Position pos) const;
    bool isPositionValid(const Position& pos) const;
    bool CollidesWith(const Building& b) const;
    bool AddBuilding(Building* b);
    void removeBuilding(Building* b);
    int  CountBuildingType(const Building& target) const;
    void addTroop(Troop* t);
    void addEnemy(Enemy* e);
    std::vector<Troop*>& getTroops()   { return troops; }
    std::vector<Enemy*>& getEnemies()  { return enemies; }
    void updateTroops();   // called each game tick
    void updateEnemies();  // called each game tick
    void cleanDead();      // removes dead enemies & troops
    void tickRaiderSpawn();// called each tick, spawns raiders on timer
    void advanceGameTick() { ++elapsedTicks; }
    int getElapsedTicks() const { return elapsedTicks; }
    void addScore(int points) { score += points; }
    int getScore() const { return score; }
    void addRaidersKilled(int count) { raidersKilled += count; }
    int getRaidersKilled() const { return raidersKilled; }

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