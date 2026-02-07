#pragma once
#include "player.h"

class Bots : public Player{
    int nextIndex;
    ActionType cycle[3] = {
        ActionType::Call,
        ActionType::Fold,
        ActionType::Raise,
    };
public:
    Bots(std::string n, int startingBalance): Player(n, startingBalance){}

    Action decideAction(int toCall, int minRaise = 10) {
        ActionType desired = cycle[nextIndex];
        nextIndex = (nextIndex + 1) % 3;

        Action act;
        
        const int balanceNow = balance;

        if (desired == ActionType::Raise) {
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