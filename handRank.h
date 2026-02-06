#pragma once
#include <vector>
#include "card.h"

enum class Rating{
    HighCard = 0,
    OnePair = 1,
    TwoPair = 2,
    ThreeOfAKind = 3,
    Straight = 4,
    Flush = 5,
    FullHouse = 6,
    FourOfAKind = 7,
    StraightFlush = 8
};

struct HandValue{
    Rating rank;
    std::vector<int> tieBreaker; //used to break ties between hands of the same rank, ordered by importance
};

class HandRank{
public:
    static HandValue evaluateHand(std::vector<Cards>& hand, std::vector<Cards>& community){
        //TODO: implement hand evaluation logic
        //consider all options C(7,5) for best 5-card hand from 7 cards
        //cannot use less than 1 card from player hand

        std::unordered_map<Cards, int> h, c;
        int idx = 0;
        for(auto i : hand){h[i] = idx++;}
        for(auto i : community){c[i] = idx++;}

        //royal flush
        Suit s; int found = 5;
        std::string rf = "AKQJT";
        for(char i : rf){
            Cards temp = {}
            if(c.find(i) == c.end()){

            }
        }


    }
};



