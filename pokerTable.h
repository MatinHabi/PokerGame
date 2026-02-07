#pragma once
#include "player.h"
#include "deck.h"
#include "helper.h"
#include "handRank.h"
#include <queue>
#include <iostream>

using namespace std;
class PokerTable{
private:
    queue<Cards> Qdeck;
    vector<Player> bots;
    vector<Cards> communityCards;
    Player* P1;
    Deck* deck;
    int pot;
public:
    PokerTable(std::string username, int startingBalance, int n = 0): deck(new Deck()), pot(0){
        //generating + shuffling deck
        deck->shuffleDeck();
        for(Cards c : deck->deck){
            Qdeck.push(c);
        }
        //generating bots
        for(int i = 0 ; i < n ; i++){
            string name = "bot" + i;
            bots.emplace_back(name, startingBalance);
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
        if(P1->isActive()){
            if(!P1->bet(bets)){
                cout << "not enough balance to play\n";
                P1->fold();
                return false;
            }
            cout << "call (c) || raise (r) || fold (f)\n";
            cout<< bets << " to play: ";
            cin >> in;
            if(in == "c"){
                if(P1->bet(bets)){
                    pot += bets;
                    cout << P1->name << " calls\n";
                    return true;
                }else{
                    cout << "not enough balance to call\n";
                    P1->fold();
                    return false;
                }
            }
            else if(in == "r"){
                int raiseAmount;
                cout << "raise amount: ";
                cin >> raiseAmount;
                while(!P1->bet(raiseAmount) || raiseAmount <= bets){
                    cout << "not enough balance to raise\n";
                    cout << "raise amount: ";
                    cin >> raiseAmount;
                }
                pot += raiseAmount;
                return true;
            }
            else if(in == "f"){
                P1->fold();
                return false;
            }
        }
        int foldcount = 0;
        for (auto& bot : bots) {
            if (!bot.isActive()) continue;
            if (bot.bet(bets)) {
                pot += bets;
            } else {
                foldcount++;
                bot.fold();
            }
        }
        if (foldcount == bots.size()) {
            cout << "All bots folded. " << P1->name << " wins!\n";
            P1->addBalance(pot);
            return false;
        }else{
            return true;
        }
        return false;
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
    
    bool showdown(){

    }
    ~PokerTable(){delete deck; delete P1;}

};