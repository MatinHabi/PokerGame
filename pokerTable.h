#pragma once
#include "player.h"
#include "deck.h"
#include "helper.h"
#include <queue>
#include <iostream>

using namespace std;
class PokerTable{
private:
    queue<Cards> Qdeck;
    vector<Player> bots;
    Player* P1;
    Deck* deck;
    int pot;
public:
    PokerTable(int n = 0): deck(new Deck()), pot(0){
        //generating + shuffling deck
        deck->shuffleDeck();
        for(Cards c : deck->deck){
            Qdeck.push(c);
        }
        //generating bots
        for(int i = 0 ; i < n ; i++){
            string name = "bot" + i;
            vector<Cards> botHand;
            for(int i = 0 ; i < 2; i++){
                botHand.push_back(Qdeck.front());
                Qdeck.pop();
            }
            bots.emplace_back(botHand,name);
        }
        //player
        string name;
        cout << "username: ";
        cin >> name;
        vector<Cards> hand;
        for(int i = 0 ; i < 2; i++){
            hand.push_back(Qdeck.front());
            Qdeck.pop();
        }
        P1 = new Player(hand, name);
    }

    void startRound() {
        // 1. Reset pot
        pot = 0;

        // 2. Reset deck
        while (!Qdeck.empty()) Qdeck.pop();
        deck->shuffleDeck();
        for (Cards c : deck->deck) {
            Qdeck.push(c);
        }

        // 3. Reset players
        P1->clearHand();
        P1->resetForRound();

        for (auto& bot : bots) {
            bot.clearHand();
            bot.resetForRound();
        }

        // 4. Deal hole cards (2 each)
        for (int i = 0; i < 2; i++) {
            P1->giveCard(Qdeck.front());
            Qdeck.pop();

            for (auto& bot : bots) {
                bot.giveCard(Qdeck.front());
                Qdeck.pop();
            }
        }

        // 5. Placeholder betting phases
        cout << "Pre-flop betting...\n";
        // bettingRound();

        cout << "Flop\n";
        // dealFlop();

        cout << "Turn\n";
        // dealTurn();

        cout << "River\n";
        // dealRiver();

        cout << "Showdown\n";
        // showdown();
    }



    ~PokerTable(){delete deck; delete P1;}

};