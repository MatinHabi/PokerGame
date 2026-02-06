#pragma once
#include <string>
#include <vector>
#include <map>
#include "card.h"
#include "chips.h"


class Player{
private:
    std::string name;
    std::vector<Cards> hand;
    std::map<Chips, int> chipBalance;
    bool active;
    int balance;
public:
    Player(std::string n = "John Doe", int startingBalance = 205)
        : name(n), balance(startingBalance), active(true) {
        chipBalance = {{Black, 1}, {Green, 2}, {Blue, 3}, {Red, 4}, {White, 5}};
    }


    void giveCard(const Cards& c) { hand.push_back(c); }
    void clearHand() { hand.clear(); }

    const std::vector<Cards>& getHand() const { return hand; }

    int getBalance() const {return balance;}
    void addBalance(int amount) {
        balance += amount;

        while(amount/100 >= 1){
            chipBalance[Black]++;
            amount -= 100;
        }
        while(amount/25 >= 1){
            chipBalance[Green]++;
            amount -= 25;
        }
        while(amount/10 >= 1){
            chipBalance[Blue]++;
            amount -= 10;
        }
        while(amount/5 >= 1){
            chipBalance[Red]++;
            amount -= 5;
        }
        while(amount/1 >= 1){
            chipBalance[White]++;
            amount -= 1;
        }
    }

    bool bet(int amount) {
        if (amount > balance) return false;
        balance -= amount;

        while(amount/100 >= 1 && chipBalance[Black] >= 1){
            chipBalance[Black]--;
            amount -= 100;
        }
        while(amount/25 >= 1 && chipBalance[Green] >= 1){
            chipBalance[Green]--;
            amount -= 25;
        }
        while(amount/10 >= 1 && chipBalance[Blue] >= 1){
            chipBalance[Blue]--;
            amount -= 10;
        }
        while(amount/5 >= 1 && chipBalance[Red] >= 1){
            chipBalance[Red]--;
            amount -= 5;
        }
        while(amount/1 >= 1 && chipBalance[White] >= 1){
            chipBalance[White]--;
            amount -= 1;
        }
        return true;
    }

    bool isActive() const { return active; }
    void fold() { active = false; }
    void resetForRound() { active = true; }

};