#pragma once

#include "player.h"
#include "deck.h"
#include "helper.h"
#include "bot.h"
#include "handRank.h"

#include <queue>
#include <vector>
#include <iostream>
#include <string>

class PokerTable {
private:
    std::queue<Cards> Qdeck;
    std::vector<Bots*> bots;                 // change Bot -> Bots if your class is named Bots
    std::vector<Cards> communityCards;

    Player* P1;
    Deck* deck;
    int pot;

    Player* getSeat(int idx) {
        // seat 0 = human, seats 1..n = bots
        if (idx == 0) return P1;
        return bots[idx - 1];
    }

    int seatCount() const {
        return 1 + static_cast<int>(bots.size());
    }

    int countActivePlayers() const {
        int active = 0;
        if (P1->isActive()) active++;
        for (auto b : bots) if (b->isActive()) active++;
        return active;
    }

    int firstActiveSeat() const {
        if (P1->isActive()) return 0;
        for (int i = 0; i < static_cast<int>(bots.size()); i++) {
            if (bots[i]->isActive()) return i + 1;
        }
        return -1;
    }

    // Turn human input into an Action (so bots + humans share same pipeline)
    Action askHumanAction(int toCall, int minRaise) {
        Action act; // defaults to FOLD/0 if you kept those defaults in Action struct

        if (toCall == 0) {
            std::cout << "check (k) || bet/raise (r) || fold (f)\n> ";
        } else {
            std::cout << "call (c) || raise (r) || fold (f)\n";
            std::cout << toCall << " to call\n> ";
        }

        std::string in;
        std::cin >> in;

        if (in == "f") {
            act.action = ActionType::Fold;
            return act;
        }

        if (in == "k") {
            act.action = ActionType::Nothing;
            return act;
        }

        if (in == "c") {
            act.action = ActionType::Call;
            return act;
        }

        if (in == "r") {
            act.action = ActionType::Raise;
            int raiseAmount;
            std::cout << "\nraise amount (min " << minRaise << "): ";
            std::cin >> raiseAmount;
            act.amount = raiseAmount;
            return act;
        }

        // fallback
        act.action = (toCall == 0) ? ActionType::Nothing : ActionType::Fold;
        return act;
    }

public:
    PokerTable(std::string username, int startingBalance, int n = 0, bool test = false)
        : P1(new Player(username, startingBalance)), deck(new Deck()), pot(0) {

        deck->shuffleDeck();
        for (Cards c : deck->deck) Qdeck.push(c);

        for (int i = 0; i < n; i++) {
            std::string name = "bot" + std::to_string(i);
            bots.push_back(new Bots(name, startingBalance, test)); // change Bot -> Bots if needed
        }
    }

    void showTableUI(bool revealBots = false) {
        std::cout << "\n=== PLAYERS ===\n";
        P1->showPlayerUI();

        for (auto& b : bots) {
            if (revealBots) {
                b->showPlayerUI();
            } else {
                // hide bot cards: print same format but X X
                std::cout << b->name
                        << " | $" << b->getBalance()
                        << " | " << (b->isActive() ? "ACTIVE" : "FOLDED")
                        << " | Hand: X X\n";
            }
        }

        std::cout << "\n=== COMMUNITY ===\n";
        printCommunityCards();
        std::cout << "Pot: $" << pot << "\n\n";
    }

    void startRound() {
        system("clear");
        pot = 0;
        communityCards.clear();

        // reset deck queue
        while (!Qdeck.empty()) Qdeck.pop();
        deck->shuffleDeck();
        for (Cards c : deck->deck) Qdeck.push(c);

        // reset players
        P1->clearHand();
        P1->resetForRound();
        for (auto& bot : bots) {
            bot->clearHand();
            bot->resetForRound();
        }

        // deal 2 hole cards each
        for (int i = 0; i < 2; i++) {
            P1->giveCard(Qdeck.front());
            Qdeck.pop();

            for (auto& bot : bots) {
                bot->giveCard(Qdeck.front());
                Qdeck.pop();
            }
        }
    }

    void printCommunityCards() {
        if (communityCards.empty()) {
            std::cout << "No community cards yet\n";
            return;
        }
        for (const auto& card : communityCards) {
            std::cout << Helper::cardToString(card) << " ";
        }
    }

    bool bettingRound(int minBet = 10) {
        if (countActivePlayers() <= 1) {
            int winnerSeat = firstActiveSeat();
            if (winnerSeat != -1) {
                Player* w = getSeat(winnerSeat);
                std::cout << w->name << " wins - everyone else folded!!\n";
                w->addBalance(pot);
            }
            return false;
        }

        const int players = seatCount();
        std::vector<int> contributed(players, 0);

        int currentBet = 0;
        int minRaise = minBet;

        int idx = 0;
        int lastAggressor = -1;
        bool someoneBet = false;

        while (true) {
            if (countActivePlayers() <= 1) {
                int winnerSeat = firstActiveSeat();
                Player* w = getSeat(winnerSeat);
                std::cout << w->name << " wins (everyone else folded)\n";
                w->addBalance(pot);
                return false;
            }

            Player* p = getSeat(idx);

            if (p->isActive()) {
                int toCall = currentBet - contributed[idx];
                if (toCall < 0) toCall = 0;

                Action a;
                if (idx == 0) {
                    a = askHumanAction(toCall, minRaise);
                } else {
                    a = bots[idx - 1]->decideAction(toCall, minRaise);
                }

                // normalize illegal check facing a bet
                if (a.action == ActionType::Nothing && toCall > 0) {
                    a.action = ActionType::Call;
                }

                if (a.action == ActionType::Fold) {
                    p->fold();
                    std::cout << p->name << " folds\n";
                }
                else if (a.action == ActionType::Nothing) {
                    std::cout << p->name << " checks\n";
                }
                else if (a.action == ActionType::Call) {
                    if (toCall == 0) {
                        std::cout << p->name << " checks\n";
                    } else if (p->bet(toCall)) {
                        contributed[idx] += toCall;
                        pot += toCall;
                        std::cout << p->name << " calls " << toCall << "\n";
                    } else {
                        p->fold();
                        std::cout << p->name << " can't call -> folds\n";
                    }
                }
                else if (a.action == ActionType::Raise) {
                    int raiseBy = a.amount;

                    // enforce minimum raise
                    if (!someoneBet) {
                        if (raiseBy < minBet) raiseBy = minBet;
                    } else {
                        if (raiseBy < minRaise) raiseBy = minRaise;
                    }

                    const int totalToPutIn = toCall + raiseBy;

                    if (p->bet(totalToPutIn)) {
                        contributed[idx] += totalToPutIn;
                        pot += totalToPutIn;

                        currentBet += raiseBy;
                        minRaise = raiseBy;
                        someoneBet = true;
                        lastAggressor = idx;

                        std::cout << p->name << " raises by " << raiseBy
                                  << " (current bet: " << currentBet << ")\n";
                    } else {
                        p->fold();
                        std::cout << p->name << " can't raise -> folds\n";
                    }
                }
            }

            // ALWAYS move to next seat (prevents infinite loop)
            idx = (idx + 1) % players;

            // End conditions
            if (!someoneBet) {
                // End after a full loop with no bets/raises
                if (idx == 0) return true;
            } else {
                // End when action comes back to last aggressor AND everyone matched currentBet
                if (idx == lastAggressor) {
                    bool allMatched = true;
                    for (int s = 0; s < players; s++) {
                        Player* ps = getSeat(s);
                        if (!ps->isActive()) continue;
                        if (contributed[s] < currentBet) {
                            allMatched = false;
                            break;
                        }
                    }
                    if (allMatched) return true;
                }
            }
        }
    }

    bool preFlopBet() {
        std::cout << "Pre-flop betting...\n";
        return bettingRound();
    }

    bool flop() {
        Qdeck.pop(); // burn
        std::cout << "Top Card Burnt! 🔥\nDealing flop...\n";

        for (int i = 0; i < 3; i++) {
            communityCards.push_back(Qdeck.front());
            std::cout << Helper::cardToString(Qdeck.front()) << " ";
            Qdeck.pop();
        }
        std::cout << "\n";

        return bettingRound();
    }

    bool dealTurn() {
        Qdeck.pop(); // burn
        std::cout << "Top Card Burnt! 🔥\nDealing turn...\n";
        printCommunityCards();

        communityCards.push_back(Qdeck.front());
        std::cout << Helper::cardToString(Qdeck.front()) << "\n";
        Qdeck.pop();

        return bettingRound();
    }

    bool dealRiver() {
        Qdeck.pop(); // burn
        std::cout << "Top Card Burnt! 🔥\nDealing river...\n";
        printCommunityCards();

        communityCards.push_back(Qdeck.front());
        std::cout << Helper::cardToString(Qdeck.front()) << "\n";
        Qdeck.pop();

        return bettingRound();
    }

    std::string showdown() {
        vector<Player*> contenders;

        if(P1->isActive()){
            contenders.push_back(P1);
        }
        
        for(auto& b : bots){
            if(!b->isActive()) continue;
            contenders.push_back(b);
        }
        vector <Player*> winners = HandRank::compareHands(contenders, communityCards);

        dealPot(winners);
        if (winners.size() > 1){
            string temp = "";
            for(auto &i : winners){
                temp += i->name + " ";
            }
            return "draw between " + temp + "!\n";
        }
        else if(winners.size() == 1) {return winners[0]->name + " wins!\n";} 
        else {cout << "ERROR\n"; return {};}
    }
    
    void dealPot(vector<Player*>& p){
        if(p.empty()){cout << "ERROR- NO WINNERS\n"; return;}
        if(p.size() == 1){p[0]->addBalance(pot);}
        else{
            int dist = pot/p.size();
            for(auto &pl : p){
                pl->addBalance(dist);
            }   
        }
        pot = 0;
    }
    
    ~PokerTable() {
        delete deck;
        delete P1;
        for (auto b : bots) delete b;
    }
};
