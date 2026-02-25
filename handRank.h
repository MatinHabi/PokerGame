#pragma once
#include <vector>
#include <set>
#include <algorithm>
#include <functional>
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
    vector<int> kickers; //used for tiebreaker
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
                if(topStraightRank == 14){
                    return {Rating::RoyalFlush, {14}};
                }
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
                if(pairs.size() >= 2){
                    return  {Rating::TwoPair, {pairs[0], pairs[1], singles[0]}};
                }else if(pairs.size() == 1){
                    temp = {pairs[0]};
                    temp.insert(temp.end(), singles.begin(), singles.end());
                    return {Rating::OnePair, {pairs[0]}};
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
        HandValue bestHand;
        vector<HandValue> allHands;
        combined.insert(combined.end(), community.begin(), community.end());
        bool first = true;
        for(int i = 0 ; i < 7 ; i++){
            for(int j = i+1 ; j < 7 ; j++){
                for(int k = j+1 ; k < 7 ; k++){
                    for(int l = k+1 ; l < 7 ; l++){
                        for(int m = l+1 ; m < 7 ; m++){
                            if(i >= 2 && j >=2 && k>=2 && l>=2 && m>=2){continue;} //must use at least 1 card from hand
                            vector<Cards> curr5 = {combined[i], combined[j], combined[k], combined[l], combined[m]};
                            allHands.push_back(eval5(curr5));
                            
                        }
                    }
                }
            }
        }
        return bestHand = better(allHands);
    }

    static HandValue better(vector<HandValue>& ah) {
        //handValue has the rating + kickers
        //Rating - just the ranking
        /*
            TODO: find the best possible hand
                - find the highest rating
                - check if any other hands share the highest rating
                - find the true highest hand by considering kickers
            Return best Hand
        */

        //FIND THE BEST RATING ===================================================
        Rating bestRating = Rating::Nothing;
        for(auto &h: ah){
            if(h.rating > bestRating){bestRating = h.rating;}
        }
        //CHECK IF BEST RATING OCCURS > 1 TIME ===================================
        vector<HandValue> sameRating;
        for(auto &h: ah){
            if(h.rating == bestRating){sameRating.push_back(h);}
        }
        //IF BEST RATING IS REOCCURING ===========================================
        if(sameRating.size() > 1){
            //there are multiple hands with the same rating
            // compare kickers -> bestHand = highest rating + highest kickers
            int bestKicker = 0;
            HandValue bestHand;
            for(auto& k : sameRating){
                int currBestKicker = *max(k.kickers.begin(), k.kickers.end());
                if(currBestKicker > bestKicker){
                    bestKicker = currBestKicker;
                    bestHand = k;
                }
            }
            return bestHand;
        }
        //IF BEST RATING IS NOT REOCCURING =======================================
        //there's only one hand with the best rating
        return sameRating[0];

    }

    static bool isDraw(vector<vector<int>>& grid) {
        if (grid.empty()) return true;

        const vector<int>& first = grid[0];

        for (int i = 1; i < grid.size(); i++) {
            if (grid[i] != first)   // vector comparison is element-wise
                return false;
        }
        return true;
    }
public:
    static vector<Player*> compareHands(vector<Player*>& contenders, std::vector<Cards>& community){
        //IF CONTENDERS IS EMPTY OR THERE IS ONLY 1 CONTENDER =============================
        if(contenders.size() == 0) return {};
        if(contenders.size() == 1) return {contenders[0]};
        //PUT EVERYONE ON A LEADERBOARD ===================================================
        vector<pair<Player*, HandValue>> leaderBoard;
        for(auto& c : contenders){
            leaderBoard.push_back({c, evaluateHand(c->hand, community)});
        }
        //FIND THE HIGHEST RATING =========================================================
        Rating maxRating = Rating::Nothing;
        for(auto& p: leaderBoard){
            if(p.second.rating > maxRating){
                maxRating = p.second.rating;
            }
        }
        //CHECK IF HIGHEST RATING IS REOCCURING ===========================================
        vector<pair<Player*, HandValue>> sameHands;
        for(auto& p : leaderBoard){
            if(p.second.rating == maxRating){
                sameHands.push_back(p);
            }
        }
        //CHECK IF BEST RATING OCCURS > 1 TIME ===================================
        vector<Player*> winners;
        if(sameHands.size() > 1){
            //DRAW CONDITION ==============================================================
            vector<vector<int>>allKickerCards;
            for(auto &p: sameHands){
                sort(p.second.kickers.begin(), p.second.kickers.end(), greater<int>());
                allKickerCards.push_back(vector<int>(p.second.kickers.begin(), p.second.kickers.end()));
            }
            if(isDraw(allKickerCards)){
                for(auto &i : sameHands){
                    winners.push_back(i.first);
                }
                return winners;
            }
            //NON DRAW CONDITION ==========================================================
                //find the highest kicker card and which player it belongs to
            auto it = max_element(allKickerCards.begin(), allKickerCards.end());
            int rowIndex = it - allKickerCards.begin();
            winners.push_back(sameHands[rowIndex].first);
        }
        //Return  ===========================================
        return winners;

    }
};



