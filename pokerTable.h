#pragma once
#include "player.h"
#include "deck.h"
#include "helper.h"
#include "bot.h"
#include "handRank.h"
#include <queue>
#include <iostream>

using namespace std;
class PokerTable{
private:
    queue<Cards> Qdeck;
    vector<Bots*> bots;
    vector<Cards> communityCards;
    Player* P1;
    Deck* deck;
    int pot;

    Player* getSeat(int idx) {
    // seat 0 = human, seats 1..n = bots
    if (idx == 0) return P1;
    return bots[idx - 1];
}

int seatCount() const {
    return 1 + (int)bots.size();
}

int countActivePlayers() const {
    int active = 0;
    if (P1->isActive()) active++;
    for (auto b : bots) if (b->isActive()) active++;
    return active;
}

int firstActiveSeat() const {
    if (P1->isActive()) return 0;
    for (int i = 0; i < (int)bots.size(); i++)
        if (bots[i]->isActive()) return i + 1;
    return -1;
}

// Turn human input into an Action (so bots + humans share same pipeline)
Action askHumanAction(int toCall, int minRaise) {
    Action act;

    if (toCall == 0) {
        cout << "bet/raise (r) || fold (f)\n";
    } else {
        cout << "call (c) || raise (r) || fold (f)\n";
        cout << toCall << " to call: ";
    }

    string in;
    cin >> in;

    if (in == "f") {
        act.action = ActionType::Fold;
        return act;
    }

    if (in == "c") {
        act.action = ActionType::Call;
        return act;
    }

    if (in == "r") {
        act.action = ActionType::Raise;
        int raiseAmount;
        cout << "raise amount (min " << minRaise << "): ";
        cin >> raiseAmount;
        act.amount = raiseAmount;
        return act;
    }

    // default safe fallback
    act.action = (toCall == 0) ? ActionType::Nothing : ActionType::Fold;
    return act;
}

public:
    PokerTable(std::string username, int startingBalance, int n = 0): deck(new Deck()), pot(0){
        //generating + shuffling deck
        deck->shuffleDeck();
        for(Cards c : deck->deck){
            Qdeck.push(c);
        }
        //generating bots
        for(int i = 0 ; i < n ; i++){
            string name = "bot" + to_string(i);
            bots.push_back(new Bots(name, startingBalance));
        }
        //player
        P1 = new Player(username, startingBalance);
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
            bot->clearHand();
            bot->resetForRound();
        }

        // 4. Deal hole cards (2 each)
        for (int i = 0; i < 2; i++) {
            P1->giveCard(Qdeck.front());
            Qdeck.pop();

            for (auto& bot : bots) {
                bot->giveCard(Qdeck.front());
                Qdeck.pop();
            }
        }
    }
    void printCommunityCards(){
        if(communityCards.empty()){
            cout << "No community cards yet\n";
            return;
        }
        for(const auto& card : communityCards){
            cout << Helper::cardToString(card) << " ";
        }
        cout << "\n";
    }
    
    bool bettingRound(int bets = 10){
        string in = "";
        if (countActivePlayers() <= 1) {
        int winnerSeat = firstActiveSeat();
        if (winnerSeat != -1) {
            Player* w = getSeat(winnerSeat);
            cout << w->name << " wins (everyone else folded)\n";
            w->addBalance(pot);
        }
        return false;
    }
    }

    bool preFlopBet(){
        cout << "Pre-flop betting...\n";
        return bettingRound();
    }

    bool flop(){
        //community cards
        Qdeck.pop(); // burn card
        cout << "Top Card Burnt! 🔥\n";
        cout << "Dealing community cards...\n";
        for(int i = 0 ; i < 3 ; i++){
            communityCards.push_back(Qdeck.front());
            cout << Helper::cardToString(Qdeck.front()) << " ";
            Qdeck.pop();
        }
        for (int i = 0 ; i < 2 ; i++){
            cout << "X ";
        }
        cout << "\n";

        return bettingRound();
    }

    bool dealTurn(){
        Qdeck.pop(); // burn card
        cout << "Top Card Burnt! 🔥\n";
        cout << "Dealing turn card...\n";
        printCommunityCards();
        cout << Helper::cardToString(Qdeck.front()) << " X\n";
        communityCards.push_back(Qdeck.front());
        Qdeck.pop();
        return bettingRound();
    }
    bool dealRiver(){
        Qdeck.pop(); // burn card
        cout << "Top Card Burnt! 🔥\n";
        cout << "Dealing river card...\n";
        printCommunityCards();
        cout << Helper::cardToString(Qdeck.front()) << "\n";
        communityCards.push_back(Qdeck.front());
        Qdeck.pop();
        return bettingRound();
    }
    
    std::string showdown(){
        Player* winner = nullptr;
        for(int i = 0 ; i < bots.size() ; i++){
            for(int j = i ; j < bots.size() ; j++){
                winner = HandRank::compareHands(bots[j], bots[i], communityCards);
            
            }
        }
        winner = HandRank::compareHands(winner, P1, communityCards);
        if(winner == P1){
            return P1->name + " wins!\n";
        }else if(winner != nullptr){
            return winner->name + " wins!\n";
        }
        return "draw!\n";
    }
    ~PokerTable(){delete deck; delete P1; for (auto b : bots) delete b;}

};