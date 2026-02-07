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
private:
    static HandValue eval5(std::vector<Cards>& hand){
        //TODO: implement hand evaluation logic
        //consider all options C(7,5) for best 5-card hand from 7 cards
        //cannot use less than 1 card from player hand

        //structure
        //counts the ranks and suits
        //then finds all conditions met and then returns the strongest condition
        
        //initalisation of vars ======================================================
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

        //flush check =============================================================
        for(int s = 1 ; s<=4 ; s++){
            if (suitCount[s] == 5){
                hasFlush = true;
                break;
            }
        }

        //straight check ===========================================================
        vector<int> uniqueRanks;
        int topStraightRank = 0;

        for(int r = 2 ; r <= 14 ; r++){
            if(rankCount[r] > 0){
                uniqueRanks.push_back(r);
            }
        }
        sort(uniqueRanks.begin(), uniqueRanks.end());

        if(uniqueRanks.size() == 5){
            if(uniqueRanks[4] - uniqueRanks[0] == 4){
                hasStraight = true;
                topStraightRank = uniqueRanks[4];
            }

            if (uniqueRanks == std::vector<int>{2,3,4,5,14}) {
                hasStraight = true;
                topStraightRank = 5; // Low Straight condition is (A-2-3-4-5)
            }
        }

        //check for duplicates ======================================================
        std::vector<int> quads, trips, pairs, singles;

        for (int i = 14; i >= 2; i--) {
            if (rankCount[i] == 4) {quads.push_back(i); hasQuads = true;} 
            else if (rankCount[i] == 3) {trips.push_back(i); hasTrips = true;}
            else if (rankCount[i] == 2) {pairs.push_back(i); hasPair = true;}
            else if (rankCount[i] == 1) {singles.push_back(i);}
        }

        //DETERMINING HAND ========================================================== 
            //strongest first
            //royal flush & Straight flush
            if(hasStraight && hasFlush){
                if(topStraightRank == 14)
                    return {Rating::RoyalFlush, {14}};
                return {Rating::StraightFlush, {topStraightRank}};
            }

            //four of a kind
            if(!quads.empty() && hasQuads){
                return {Rating::FourOfAKind, {quads[0], singles[0]}}; //5 cars hand -> return {repeated rank, extra card}
            }

            //full house
            if(!trips.empty() && !pairs.empty()){
                return {Rating::FullHouse, {trips[0], pairs[0]}};
            }

            //flush
            if(hasFlush){
                return {Rating::Flush, ranks};
            }

            //straight
            if(hasStraight){
                return {Rating::Straight, {topStraightRank}};
            }

            //three of a kind
            if(!trips.empty() && hasTrips){
                vector<int> temp = {trips[0]};
                temp.insert(temp.end(), singles.begin(), singles.end());
                return {Rating::FourOfAKind, temp}; //5 cars hand -> return {repeated rank, extra cards}
            }

            //two pair
            if(pairs.size() >= 1 && hasPair){
                vector<int> temp;
                if(pairs.size() == 1){
                    temp = {pairs[0]};
                    temp.insert(temp.end(), singles.begin(), singles.end());
                    return {Rating::OnePair, {pairs[0]}};
                }
                if(pairs.size() >= 2){
                    return  {Rating::TwoPair, {pairs[0], pairs[1], singles[0]}};
                }
            }

            //one pair
            if (pairs.size() == 1) {
                std::vector<int> keys = {pairs[0]};
                keys.insert(keys.end(), singles.begin(), singles.end());
                return {Rating::OnePair, keys};
            }
            
            // High Card
            return {Rating::HighCard, ranks};

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
public:
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



