#pragma once
#include "card.h"
#include <algorithm>
#include <iostream>

class Helper{
public:
    void static printCards(std::vector<Cards>& x){
        for(const auto& i : x){
            std::string out = std::to_string(i.rank) + ntos[i.suit];
            std::cout << out << " ";
        }
        std::cout << "\n";
    }
};