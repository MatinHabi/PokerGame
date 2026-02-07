#include "player.h"

    Player::Player(std::string n , int startingBalance):
                name(n), balance(startingBalance){};
    void Player::giveCard(const Cards& c) { hand.push_back(c); }
    void Player::clearHand() { hand.clear(); }

    std::vector<Cards>& Player::getHand(){return hand;}

    int Player::getBalance() const{return balance;}
    void Player::addBalance(int amount) {
        balance += amount;

        while(amount/100 >= 1){
            chipBalance[Black]++;
            amount -= 100;
        }
        while(amount/25 >= 1){
            chipBalance[Green]++;
            amount -= 25;
        }
        while(amount/10 >= 1){
            chipBalance[Blue]++;
            amount -= 10;
        }
        while(amount/5 >= 1){
            chipBalance[Red]++;
            amount -= 5;
        }
        while(amount/1 >= 1){
            chipBalance[White]++;
            amount -= 1;
        }
    }

    bool Player::bet(int amount) {
        if (amount <= 0) return false;
        if (amount > balance) return false;
        balance -= amount;

        while(amount/100 >= 1 && chipBalance[Black] >= 1){
            chipBalance[Black]--;
            amount -= 100;
        }
        while(amount/25 >= 1 && chipBalance[Green] >= 1){
            chipBalance[Green]--;
            amount -= 25;
        }
        while(amount/10 >= 1 && chipBalance[Blue] >= 1){
            chipBalance[Blue]--;
            amount -= 10;
        }
        while(amount/5 >= 1 && chipBalance[Red] >= 1){
            chipBalance[Red]--;
            amount -= 5;
        }
        while(amount/1 >= 1 && chipBalance[White] >= 1){
            chipBalance[White]--;
            amount -= 1;
        }
        return true;
    }

    bool Player::isActive() const { return active; }
    void Player::fold() { active = false; }
    void Player::resetForRound() { active = true; }
    void Player::showPlayerUI(){
        
    }