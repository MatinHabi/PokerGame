#include <iostream>
#include "helper.h"
#include "card.h"
#include "pokerTable.h"
#include "bot.h"
#include "deck.h"
using namespace std;

int main() {
    system("clear");
    PokerTable table("Matin", 200, 3); // username, startingBalance, numberOfBots

    char again = 'y';
    while (again == 'y' || again == 'Y') {
        table.startRound();

        bool cont = true;

        cont = table.preFlopBet();
        if (cont) cont = table.flop();
        if (cont) cont = table.dealTurn();
        if (cont) cont = table.dealRiver();

        if (cont) {
            std::cout << "\n--- SHOWDOWN ---\n";
            std::cout << table.showdown();
        } else {
            std::cout << "\n--- ROUND ENDED (fold win) ---\n";
        }

        std::cout << "\nPlay again? (y/n): ";
        std::cin >> again;
        std::cout << "\n";
    }

    return 0;
}