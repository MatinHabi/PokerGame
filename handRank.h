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
        for(int i = 2 ; i <= 10 ; i++){
            bool straight = true;
            for(int j = 1 ; j <= 4 ; j++){
                if(rankCount[i+j] == 0){
                    straight = false;
                    break;
                }
            }
            if(straight){
                hasStraight = true; 
                break;
            }
        }

        //edge case: low straight (A-2-3-4-5)
        if (!hasStraight &&
        rankCount[14] &&
        rankCount[2] &&
        rankCount[3] &&
        rankCount[4] &&
        rankCount[5]) {
            hasStraight = true;
        }

        //flush
        for(int i = 1 ; i <= 4; i++){
            if(suitCount[i] >= 5){
                hasFlush = true;
                break;
            }
        }

        //straight flush & royal flush
        if(hasStraight && hasFlush){
            hasStraightFlush = true;
            //royal flush is 10 J K Q A

            if (rankCount[10] &&
            rankCount[11] &&
            rankCount[12] &&
            rankCount[13] &&
            rankCount[14]) {
                hasRoyalFlush = true;
            }
        }

        //final rankings

        if (hasRoyalFlush)      return Rating::RoyalFlush;
        if (hasStraightFlush)   return Rating::StraightFlush;
        if (hasQuads)           return Rating::FourOfAKind;
        if (hasTrips && hasPair) return Rating::FullHouse;
        if (hasFlush)           return Rating::Flush;
        if (hasStraight)        return Rating::Straight;
        if (hasTrips)           return Rating::ThreeOfAKind;
        if (hasTwoPair)         return Rating::TwoPair;
        if (hasPair)            return Rating::OnePair;
        
        return Rating::HighCard;
    }
};



