#pragma once
#include "player.h"
#include <random>

class Bots : public Player{
private:
    ActionType cycle[4] = {
        ActionType::Nothing,
        ActionType::Raise,
        ActionType::Call,
        ActionType::Fold,
    };
    static std::mt19937& rng() {
        static std::mt19937 gen(std::random_device{}());
        return gen;
    }
public:
    Bots(std::string n, int startingBalance): Player(n, startingBalance){}

    Action decideAction(int toCall, int minRaise = 10) {
        std::uniform_int_distribution<int> dist(1, 4);
        int roll = dist(rng());

        ActionType desired =cycle[roll-1];

        Action act;

        const int balanceNow = balance;
        
        if (desired == ActionType::Nothing) {
            // "Nothing" = CHECK if possible, else CALL if possible, else FOLD
            if (toCall == 0) act.action = ActionType::Nothing; // or ActionType::Check if you have it
            else if (balanceNow >= toCall) act.action = ActionType::Call;
            else act.action = ActionType::Fold;
        }
        else if (desired == ActionType::Raise) {
            // Raise is always +10 over Calling
            if (balanceNow >= toCall + 10) {
                act.action = ActionType::Raise;
                act.amount = 10;
            } else {
                act.action = ActionType::Fold;
            }
        }
        else if (desired == ActionType::Call) {
            if (balanceNow >= toCall) {
                act.action = ActionType::Call;
            } else {
                act.action = ActionType::Fold;
            }
        }
        else { // desired == Fold
            act.action = ActionType::Fold;
        }

        return act;
    }
};