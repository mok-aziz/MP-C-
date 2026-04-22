#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <conio.h>
#include <windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#include "Board.h"
#include "Entities/Player.h"
#include "Buildings/Wall.h"
#include "Buildings/TownHall.h"
#include "Buildings/GoldMine.h"
#include "Buildings/ElixirCollector.h"
#include "Buildings/Barrack.h"          // NEW
#include "Entities/Troops/Archer.h"
#include "Entities/Troops/Barbarian.h"

const int MAP_W = 40;
const int MAP_H = 20;

void clearScreen() { system("cls"); }

void showGameOverScreen(const std::string& message, int score, int raidersKilled) {
    clearScreen();
    std::cout
        << "+--------------------------------------------------------------+\n"
        << "|                                                              |\n"
        << "|                         GAME OVER                            |\n"
        << "|                                                              |\n"
        << "|                  " << message << "\n"
        << "|                  Score: " << score << "\n"
        << "|                  Raiders killed: " << raidersKilled << "\n"
        << "|                                                              |\n"
        << "+--------------------------------------------------------------+\n\n"
        << "  Press any key to exit...\n";
    _getch();
}

void renderFrame(Board& board, const Player* player) {
    const Resources& res = player->getResources();
    TownHall* th = board.getTownHall();

    std::string thHp = th ? std::to_string(th->getHealth()) : "NOT BUILT";

    // Build right panel
    std::vector<std::string> panel = {
        "| Gold:     " + std::to_string(res.getGold()),
        "| Elixir:   " + std::to_string(res.getElixir()),
        "| TownHall: " + thHp,
        "| ---------",
    };

    int archerCount = 0;
    int barbarianCount = 0;
    for (Troop* t : board.getTroops()) {
        if (dynamic_cast<Archer*>(t)) ++archerCount;
        else if (dynamic_cast<Barbarian*>(t)) ++barbarianCount;
    }
    panel.push_back("|  Score:     " + std::to_string(board.getScore()));
    
    panel.push_back("|  Archers 🏹:    " + std::to_string(archerCount));
    panel.push_back("|  Barbarians 🐕: " + std::to_string(barbarianCount));

    // Show enemy count
    int enemyCount = (int)board.getEnemies().size();
    panel.push_back("|  Raiders 👺:  " + std::to_string(enemyCount));
    panel.push_back("|  Raiders killed: " + std::to_string(board.getRaidersKilled()));
    

    std::string hBorder = "+";
    for (int i = 0; i < MAP_W * 2; i++) hBorder += "-";
    hBorder += "+";

    auto gridLines = board.getRenderLines();
    int pIdx = 0;
    auto printRow = [&](const std::string& row) {
        std::cout << row;
        if (pIdx < (int)panel.size())
            std::cout << "  " << panel[pIdx++];
        std::cout << "\n";
    };

    printRow(hBorder);
    for (auto& line : gridLines) printRow(line);
    printRow(hBorder);

    while (pIdx < (int)panel.size()) {
        std::cout << std::string(MAP_W * 2 + 2, ' ')
                  << "  " << panel[pIdx++] << "\n";
    }

    std::cout << "\nArrows=Move  C=Collect\n";
    std::cout << "G=GoldMine ( 100 elixir)  E=ElixirCollector (100 gold)\n";
    std::cout << "W=Wall (10 gold)  T=TownHall (100 gold)  B=Barrack/Enter (150 gold)  Q=Quit\n";
}

int main() {
    SetConsoleOutputCP(65001);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    srand((unsigned)time(nullptr));

    Board board(MAP_W, MAP_H);
    Player* player = new Player(Position(MAP_W / 2, MAP_H / 2));
    board.setPlayer(player);

    bool running = true;

    while (running) {

        // ── Check game-over ───────────────────────────────────────────
        TownHall* th = board.getTownHall();
        if (th && th->getHealth() <= 0) {
            showGameOverScreen("Your TownHall was destroyed!", board.getScore(), board.getRaidersKilled());
            break;
        }

        // ── Input ─────────────────────────────────────────────────────
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 0 || ch == 224) {
                ch = _getch();
                switch (ch) {
                    case 72: player->moving(board, 0, -1);  break;
                    case 80: player->moving(board, 0,  1);  break;
                    case 75: player->moving(board, -1, 0);  break;
                    case 77: player->moving(board,  1, 0);  break;
                }
            } else {
                switch (ch) {
                    case 'q': case 'Q':
                        showGameOverScreen("You quit the game.", board.getScore(), board.getRaidersKilled());
                        running = false;
                        break;

                    case 'c': case 'C':
                        player->Interact(board);
                        break;

                    case 'w': case 'W': {
                        Wall* w = new Wall(player->getPosition());
                        if (!board.AddBuilding(w)) delete w;
                        break;
                    }
                    case 'g': case 'G': {
                        GoldMine* g = new GoldMine(player->getPosition());
                        if (!board.AddBuilding(g)) delete g;
                        break;
                    }
                    case 'e': case 'E': {
                        ElixirCollector* ec = new ElixirCollector(player->getPosition());
                        if (!board.AddBuilding(ec)) delete ec;
                        break;
                    }
                    case 't': case 'T': {
                        TownHall* newTh = new TownHall(player->getPosition());
                        if (!board.AddBuilding(newTh)) delete newTh;
                        break;
                    }

                    // ── NEW: Barrack ──────────────────────────────
                    case 'b': case 'B': {
                        // If player is standing on a Barrack, open its menu
                        Barrack* foundBarrack = nullptr;
                        for (Building* b : board.getBuildings()) {
                            if (auto* bar = dynamic_cast<Barrack*>(b)) {
                                if (bar->collidesWith(player->getPosition())) {
                                    foundBarrack = bar;
                                    break;
                                }
                            }
                        }
                        if (foundBarrack) {
                            foundBarrack->openMenu(board, player);
                        } else {
                            Barrack* newBar = new Barrack(player->getPosition());
                            if (!board.AddBuilding(newBar)) delete newBar;
                        }
                        break;
                    }
                }
            }
        }

        // ── Update buildings (resource generation) ────────────────────
        for (Building* b : board.getBuildings()) b->update(board);

        board.advanceGameTick();

        // ── Update troops & enemies ───────────────────────────────────
        board.updateEnemies();
        board.updateTroops();
        board.cleanDead();

        // ── Raider spawn timer ────────────────────────────────────────
        board.tickRaiderSpawn();

        // ── Render ────────────────────────────────────────────────────
        clearScreen();
        renderFrame(board, player);

        Sleep(100);
    }

    std::cout << "Goodbye!\n";
    delete player;
    return 0;
}