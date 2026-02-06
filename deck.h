#pragma once
#include <random>
#include <algorithm>
#include "card.h"

class Deck{
protected:
    std::vector<Cards> deck;
    int next;
    friend class Helper;
    friend class PokerTable;
public:
    Deck(): next(0){reset();}

    void reset(){
        deck.clear();
        for(int s = 0 ; s < 4 ; s++){
            for(int r = 2 ; r <= 14 ; r++){
                deck.push_back({r,(Suit)s});
            }
        }
    }

    void shuffleDeck(){
        if(deck.empty() || deck.size() < 52){return;}
        //these are static to improve run-time performance - no need to recreate variables every time
        static std::random_device rd; //non-deterministic entropy source
        static std::mt19937 rng(rd()); //RNG with unpredic table seen based on rd

        std::shuffle(deck.begin(), deck.end(), rng); //Fisher-Yates shuffle -> removes bias
    }

    std::vector<Cards> getDeck(){return deck;}
    
    Cards deal(){
        if(next<=52){
            return deck[next++];
        }
        return {0,(Suit)1};
    }

    ~Deck(){}

};