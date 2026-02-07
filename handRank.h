#pragma once
#include <vector>
#include <unordered_set>
#include <algorithm>
#include "card.h"
#include "helper.h"
#include "player.h"
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

struct HandValue{
    Rating rating;
    vector<int> keys; //used for tiebreaker
};

class HandRank{
public:
    static HandValue eval5(std::vector<Cards>& hand){
        //TODO: implement hand evaluation logic
        //consider all options C(7,5) for best 5-card hand from 7 cards
        //cannot use less than 1 card from player hand
        HandValue bestHand = {Rating::Nothing, {}};
        int rankCount[15] = {0};
        int suitCount[5] = {0};
        vector<int> ranks;

        for(auto h: hand){
            rankCount[h.rank]++;
            suitCount[h.suit]++;
            ranks.push_back(h.rank);
        }

        sort(ranks.begin(), ranks.end(), greater<int>());

        bool hasPair = false;
        bool hasTwoPair = false;
        bool hasTrips = false;
        bool hasQuads = false;
        bool hasStraight = false;
        bool hasFlush = false;
        bool hasStraightFlush = false;
        bool hasRoyalFlush = false;

        //flush check
        for(int s = 1 ; s<=4 ; s++){
            if (suitCount[s] == 5){
                hasFlush = true;
                break;
            }
        }

        //straight check
    }

    static HandValue evaluateHand(std::vector<Cards>& hand, std::vector<Cards>& community){
        vector<Cards> combined = hand;
        HandValue bestHand = {Rating::Nothing, {}};
        combined.insert(combined.end(), community.begin(), community.end());
        bool first = true;
        for(int i = 0 ; i < 7 ; i++){
            for(int j = i+1 ; j < 7 ; j++){
                for(int k = j+1 ; k < 7 ; k++){
                    for(int l = k+1 ; l < 7 ; l++){
                        for(int m = l+1 ; m < 7 ; m++){
                            if(i >= 2 && j >=2 && k>=2 && l>=2 && m>=2){continue;} //must use at least 1 card from hand

                            vector<Cards> curr5 = {combined[i], combined[j], combined[k], combined[l], combined[m]};

                            HandValue currHandValue = eval5(curr5);

                            if(first || better(currHandValue, bestHand)){
                                bestHand = currHandValue;
                                first = false;
                            }
                            
                        }
                    }
                }
            }
        }
        return bestHand;
    }

    static bool better(const HandValue& a, const HandValue& b) {
        if (a.rating != b.rating)
            return a.rating > b.rating;
        return a.keys > b.keys;
    }

    static Player* compareHands(Player* p1, Player* p2, std::vector<Cards>& community){
        HandValue r1 = evaluateHand(p1->getHand(), community);
        HandValue r2 = evaluateHand(p2->getHand(), community);

        if (r1.rating != r2.rating)
            return r1.rating > r2.rating ? p1 : p2;

        if (r1.keys != r2.keys)
            return r1.keys > r2.keys ? p1 : p2;

        return nullptr; // tie
        
    }
};



