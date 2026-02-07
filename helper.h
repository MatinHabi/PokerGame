#pragma once
#include "card.h"
#include "deck.h"
#include <algorithm>
#include <iostream>
#include <vector>

class Helper{
private:
    static const std::vector<std::string> ranks;
    static const std::vector<std::string> suits;
public:
    void static printCards(std::vector<Cards>& x){
        for(const auto& i : x){
            std::string out = ranks[i.rank-2] + suits[i.suit];
            std::cout << out << " ";
        }
        std::cout << "\n";
    }

    std::string static cardToString(Cards c){
        return ranks[c.rank-2] + suits[c.suit];
    }

    std::string static deal(Deck* d){
        Cards out = d->deal();
        if(out.rank == 0){std::cout<<"ERROR\n";return "";}
        return ranks[out.rank-2] + suits[out.suit];
    }
};