#pragma once
#include <vector>
#include <unordered_set>
#include "card.h"
using namespace std;

enum class Rating{
    Nothing = -1,
    HighCard = 0,
    OnePair = 1,
    TwoPair = 2,
    ThreeOfAKind = 3,
    Straight = 4,
    Flush = 5,
    FullHouse = 6,
    FourOfAKind = 7,
    StraightFlush = 8,
    RoyalFlush = 9,
};

class HandRank{
public:
    static Rating evaluateHand(std::vector<Cards>& hand, std::vector<Cards>& community){
        //TODO: implement hand evaluation logic
        //consider all options C(7,5) for best 5-card hand from 7 cards
        //cannot use less than 1 card from player hand

        int rankCount[15] = {0};
        int suitCount[5] = {0};

        for(auto h: hand){
            rankCount[h.rank]++;
            suitCount[h.suit]++;
        }

        for(auto c: community){
            rankCount[c.rank]++;
            suitCount[c.suit]++;
        }

        Rating result = Rating::Nothing;
        bool tPair = false, rf = false;

        bool hasPair = false;
        bool hasTwoPair = false;
        bool hasTrips = false;
        bool hasQuads = false;
        bool hasStraight = false;
        bool hasFlush = false;
        bool hasStraightFlush = false;
        bool hasRoyalFlush = false;

        int pairCount = 0;

        for(int i = 2 ; i <= 14 ; i++){
            if(rankCount[i] == 4){hasQuads = true;}
            if(rankCount[i] == 3){hasTrips = true;}
            if(rankCount[i] == 2){pairCount++;}
        }

        hasPair = pairCount >= 1;
        hasTwoPair = pairCount >= 2;

        //straight detection
        
    }
};



