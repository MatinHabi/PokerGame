#pragma once
#include <string>
#include <vector>
#include <map>
#include "card.h"
#include "Action.h"
#include "helper.h"
#include "chips.h"


class Player{
private:
    std::string name;
    std::vector<Cards> hand;
    std::map<Chips, int> chipBalance;
    bool active;
    int balance;
    friend class PokerTable;
    friend class HandRank;
    friend class Bots;  
public:
    Player(std::string n = "John Doe", int startingBalance = 205);


    void giveCard(const Cards& c);
    void clearHand();

    std::vector<Cards>& getHand();

    int getBalance() const;

    void addBalance(int amount);

    bool bet(int amount);

    bool isActive() const;

    void fold();

    void resetForRound();

    virtual Action decideAction(int toCall, int minRaise) { 
        return Action{ActionType::Nothing, 0}; 
    }

    virtual ~Player() = default;
    void showPlayerUI();
};