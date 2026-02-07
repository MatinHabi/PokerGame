#include <iostream>
#include "helper.h"
#include "card.h"
#include "pokerTable.h"
#include "bot.h"
#include "deck.h"
using namespace std;

int main() {
    system("clear");
    string name = "";
    cout << "Enter Username: ";
    cin >> name;
    PokerTable table(name, 200, 3);

    char again = 'y';
    while (again == 'y' || again == 'Y') {
        table.startRound();

        // After hole cards dealt
        table.showTableUI(false);

        bool cont = true;

        cont = table.preFlopBet();
        if (cont) {
            system("clear");
            table.showTableUI(false);
            cont = table.flop();
        }
        if (cont) {
            system("clear");
            table.showTableUI(false);
            cont = table.dealTurn();
        }
        if (cont) {
            system("clear");
            table.showTableUI(false);
            cont = table.dealRiver();
        }

        if (cont) {
            std::cout << "\n--- SHOWDOWN ---\n";
            table.showTableUI(true);          // reveal bot hands
            std::cout << table.showdown();
        } else {
            std::cout << "\n--- ROUND ENDED (fold win) ---\n";
            table.showTableUI(true);          // reveal for debugging
        }

        std::cout << "\nPlay again? (y/n): ";
        std::cin >> again;
        std::cout << "\n";
    }

    return 0;
}