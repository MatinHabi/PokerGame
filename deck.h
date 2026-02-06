#pragma once
#include <vector>
#include <algorithm>
#include <random>
#include "card.h"

class Deck{
protected:
    std::vector<Cards> deck;
    int next;
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
        return deck[next++];
    }



};