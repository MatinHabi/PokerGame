#pragma once
#include <unordered_map>
#include <string>

enum Suit {Clubs=1, Diamonds, Hearts, Spades};

struct Cards{
    int rank; //val is between 2-14
    Suit suit;
};