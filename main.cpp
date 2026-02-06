#include <iostream>
#include "helper.h"
#include "card.h"
#include "deck.h"
using namespace std;
int main(){
    Deck* d1 = new Deck();
    d1->shuffleDeck();
    auto x = d1->getDeck();
    Helper::printCards(x);

    cout << "deal test\n";
    for(int i = 0 ; i < 53 ; i++){
        cout << Helper::deal(d1) << " \n";
    }
}