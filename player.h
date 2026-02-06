#pragma once
#include <string>
#include <vector>
#include "card.h"

enum class Moves{
    Nothing,
    Fold,
    Check,
    Raise,
};

class Player{
private:
    std::string name;
    std::vector<Cards> hand;
    int balance;
};