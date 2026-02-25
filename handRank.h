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
    vector<int> handCards;
    vector<int> kickers; //used for tiebreaker
};

class HandRank{
private:
    static HandValue eval5(std::vector<Cards>& hand){
        if(hand.empty()){std::cerr<< "ERROR- ALL HANDS VECTOR EMPTY!\n";}
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

        //suit check
        bool sameSuit = false;
        for(int &i : suitCount){
            if(i == 5) sameSuit = true;
        }

        //rank check
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
            } else if (uniqueRanks == std::vector<int>{2, 3, 4, 5, 14}) {
                hasStraight = true;
                topStraightRank = 5; // Wheel (A-2-3-4-5)
            }
        }

        //check for duplicates ======================================================
        std::vector<int> quads, trips, pairs, singles;

        for (int i = 14; i >= 2; i--) {
            if (rankCount[i] == 1) {singles.push_back(i);}
            if (rankCount[i] == 2) {pairs.push_back(i);}
            if (rankCount[i] == 3) {trips.push_back(i);} 
            if (rankCount[i] == 4) {quads.push_back(i);}
        }

        //DETERMINING HAND ========================================================== 
            //strongest first
            //royal flush & Straight flush
            if(hasStraight && hasFlush){
                if(topStraightRank == 14){
                    return {Rating::RoyalFlush, {14}, {}};
                }
                if(hasStraightFlush) {
                    return {Rating::StraightFlush, {topStraightRank}, {}};
                }
            }

            //four of a kind
            if(!quads.empty()){
                return {Rating::FourOfAKind, {quads[0]}, {singles[0]}}; //5 cars hand -> return {repeated rank, extra card}
            }

            //full house
            if(!trips.empty() && !pairs.empty()){
                return {Rating::FullHouse, {trips[0], pairs[0]}, {}};
            }

            //flush
            if(hasFlush){
                return {Rating::Flush, ranks, {}};
            }

            //straight
            if(hasStraight){
                return {Rating::Straight, {topStraightRank}, {}};
            }

            //three of a kind
            if(!trips.empty() && singles.size() == 2){
                return {Rating::ThreeOfAKind, {trips[0]}, {singles[0], singles[1]}}; //5 cars hand -> return {repeated rank, extra cards}
            }

            //two pair
            if(pairs.size() >= 2){
                return {Rating::TwoPair, {pairs[0], pairs[1]}, {singles[0]}};
            }

            //one pair
            if (pairs.size() == 1 && singles.size() == 3) {
                return {Rating::OnePair, {pairs[0]}, {singles[0], singles[1], singles[2]}};
            }

            // High Card
            return {Rating::HighCard, {ranks[0]}, {ranks[1], ranks[2], ranks[3], ranks[4]}};

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
                            //if(i >= 2 && j >=2 && k>=2 && l>=2 && m>=2){continue;} //must use at least 1 card from hand
                            vector<Cards> curr5 = {combined[i], combined[j], combined[k], combined[l], combined[m]};
                            allHands.push_back(eval5(curr5));
                            
                        }
                    }
                }
            }
        }
        return bestHand = bestOfAllHands(allHands);
    }

    static bool compareHandValue(const HandValue& a, const HandValue& b){
        //return best hand or compare kickers
        if(a.rating != b.rating){return a.rating > b.rating;}
        if(a.handCards != b.handCards){return a.handCards > b.handCards;}
        return compareKickers(a,b);
    }

    static bool compareKickers(const HandValue& a, const HandValue& b){
        if(a.kickers != b.kickers){return a.kickers > b.kickers;}
        //std::cout<<"SAME HAND\n";
        return false;
    }

    static HandValue bestOfAllHands(vector<HandValue>& ah) {
        if (ah.empty()) {
            // Defensive fallback (shouldn't happen if evaluateHand constructed hands)
            return HandValue{Rating::HighCard, {}, {}};
        }

        //normalise
        for(auto &a : ah){
            sort(a.handCards.begin(), a.handCards.end(), greater<int>());
            sort(a.kickers.begin(), a.kickers.end(), greater<int>());
        }

        //sort
        sort(ah.begin(), ah.end(), 
            [](const HandValue& a, const HandValue& b){
                return compareHandValue(a,b);
            }
        );
        return ah[0];
    }

public:

    static vector<Player*> compareHands(vector<Player*>& contenders, std::vector<Cards>& community){
        //IF CONTENDERS IS EMPTY OR THERE IS ONLY 1 CONTENDER =============================
        if(contenders.size() == 0) return {};
        if(contenders.size() == 1) return {contenders[0]};
        //leaderboard
        vector<pair<Player*, HandValue>> leaderboard;
        for(auto &p: contenders){
            auto v = evaluateHand(p->hand, community);
            sort(v.handCards.begin(), v.handCards.end(), greater<int>());
            sort(v.kickers.begin(), v.kickers.end(), greater<int>());
            leaderboard.push_back({p,v});
        }
        //sort leaderboard
        sort(leaderboard.begin(), leaderboard.end(), 
            [](const pair<Player*, HandValue> &a, const pair<Player*, HandValue>& b){
                return compareHandValue(a.second,b.second);
            }
        );

        vector<Player*> winners;
        const HandValue& bestHand = leaderboard[0].second;

        for (size_t i = 0; i < leaderboard.size(); i++) {
            // Tie detection: If A is not better than B AND B is not better than A
            if (!compareHandValue(leaderboard[i].second, bestHand) && 
                !compareHandValue(bestHand, leaderboard[i].second)) {
                winners.push_back(leaderboard[i].first);
            } else {
                break; // List is sorted, so we can stop once someone loses
            }
        }

        return winners;
    }
};



