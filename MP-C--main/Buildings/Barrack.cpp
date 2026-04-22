#include "Barrack.h"
#include "../Board.h"
#include "../Entities/Player.h"
#include "../Entities/Troops/Archer.h"
#include "../Entities/Troops/Barbarian.h"
#include "../Buildings/TownHall.h"
#include <iostream>
#include <conio.h>
#include <windows.h>

Barrack::Barrack(Position pos)
    : Building(pos, 3, 3, "🏠", Resources(150, 0), 1, 200) {}

void Barrack::openMenu(Board& board, Player* player) {
    TownHall* th = board.getTownHall();
    if (!th) {
        system("cls");
        std::cout << "\n\n  [!] You need a TownHall before training troops!\n";
        std::cout << "      Press any key to continue...\n";
        _getch();
        return;
    }

    Position barrackPos = getPosition();
    Position townHallPos = th->getPosition();

    auto troopSpawnPosition = [&](int index) {
        static const Position offsets[] = {
            Position(0, 0), Position(1, 0), Position(-1, 0),
            Position(0, 1), Position(0, -1), Position(1, 1),
            Position(-1, 1), Position(1, -1), Position(-1, -1)
        };
        return barrackPos + offsets[index % (int)(sizeof(offsets) / sizeof(offsets[0]))];
    };

    while (true) {
        system("cls");

        auto& troops = board.getTroops();
        int count = (int)troops.size();

        std::cout << "\n";
        std::cout << "  ======================================\n";
        std::cout << "          🏠  ["
                  << count << "/" << CAPACITY << " slots]\n";
        std::cout << "  ======================================\n";

        // Show filled / empty slots
        std::cout << "  Slots:  ";
        for (Troop* t : troops)
            std::cout << t->getRepr() << " ";
        for (int i = count; i < CAPACITY; i++)
            std::cout << "[  ] ";
        std::cout << "\n";
        std::cout << "  ======================================\n";

        if (count < CAPACITY) {
            std::cout << "  1  =  Archer      (50 gold)\n";
            std::cout << "  2  =  Barbarian   (30 gold)\n";
        } else {
            std::cout << "  Slots full — exit and deploy your troops.\n";
        }
        std::cout << "  Q  =  Exit & Deploy\n";
        std::cout << "  ======================================\n";
        std::cout << "  Your gold: "
                  << player->getResources().getGold() << "\n\n";

        int ch = _getch();

        if (ch == 'q' || ch == 'Q') break;

        if (count >= CAPACITY) continue;

        if (ch == '1') {
            Resources cost(50, 0);
            if (player->getResources().canAfford(cost)) {
                player->getResources().consume(cost);
                Archer* a = new Archer(troopSpawnPosition(count));
                a->setHome(townHallPos);
                a->setState(TroopState::RETURNING);
                board.addTroop(a);
            } else {
                std::cout << "  Not enough gold!\n";
                Sleep(800);
            }
        } else if (ch == '2') {
            Resources cost(30, 0);
            if (player->getResources().canAfford(cost)) {
                player->getResources().consume(cost);
                Barbarian* b = new Barbarian(troopSpawnPosition(count));
                b->setHome(townHallPos);
                b->setState(TroopState::RETURNING);
                board.addTroop(b);
            } else {
                std::cout << "  Not enough gold!\n";
                Sleep(800);
            }
        }
    }
}