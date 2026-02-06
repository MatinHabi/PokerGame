#pragma once
#include <unordered_map>
enum Suit {Clubs, Diamonds, Hearts, Spades};
std::unordered_map<Suit, std::string> ntos{{Hearts,"♥"},{Clubs,"♠"},{Diamonds,"♦"},{Spades,"♣"}};
struct Cards{
    int rank; //val is between 2-14
    Suit suit;
};