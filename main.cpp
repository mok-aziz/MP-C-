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
const int MAP_W = 40;
const int MAP_H = 20;

void clearScreen() { system("cls"); }

void renderFrame(Board& board, const Player* player) {
    const Resources& res = player->getResources();

    std::string thHp = board.getTownHall()
        ? std::to_string(board.getTownHall()->getHealth())
        : "NOT BUILT";

    std::vector<std::string> panel = {
        "| Gold:     " + std::to_string(res.getGold()),
        "| Elixir:   " + std::to_string(res.getElixir()),
        "| TownHall: " + thHp,
        "| ---------",
    };

    std::string hBorder = "+";
    for (int i = 0; i < MAP_W * 2; i++) hBorder += "-";
    hBorder += "+";

    auto gridLines = board.getRenderLines();

    int pIdx = 0;
    auto printRow = [&](const std::string& row) {
        std::cout << row;
        if (pIdx < (int)panel.size())
            std::cout << "   " << panel[pIdx++];
        std::cout << "\n";
    };

    printRow(hBorder);
    for (auto& line : gridLines) printRow(line);
    printRow(hBorder);

    while (pIdx < (int)panel.size()) {
        std::cout << std::string(MAP_W * 2 + 2, ' ')
                  << "   " << panel[pIdx++] << "\n";
    }

    std::cout << "\nArrows=Move  C=Collect  G=GoldMine  E=ElixirCollector  W=Wall  T=TownHall  Q=Quit\n";
}

int main() {
    SetConsoleOutputCP(65001);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    srand((unsigned)time(nullptr));

    Board board(MAP_W, MAP_H);
    // ── Board starts empty ──
    Player* player = new Player(Position(MAP_W / 2, MAP_H / 2));
    board.setPlayer(player);

    bool running = true;

    while (running) {

        // ── Input ─────────────────────────────────────────
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 0 || ch == 224) {
                ch = _getch();
                switch (ch) {
                    case 72: player->moving(board,  0, -1); break; // UP
                    case 80: player->moving(board,  0,  1); break; // DOWN
                    case 75: player->moving(board, -1,  0); break; // LEFT
                    case 77: player->moving(board,  1,  0); break; // RIGHT
                }
            } else {
                switch (ch) {
                    case 'q': case 'Q': running = false; break;
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
                        TownHall* th = new TownHall(player->getPosition());
                        if (!board.AddBuilding(th)) delete th;
                        break;
                    }
                }
            }
        }

        // ── Update buildings (resource generation) ────────
        for (Building* b : board.getBuildings()) b->update(board);

        // ── Render ────────────────────────────────────────
        clearScreen();
        renderFrame(board, player);

        Sleep(100);
    }

    std::cout << "Goodbye!\n";
    delete player;
    return 0;
}