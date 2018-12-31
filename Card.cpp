

#include "Card.h"


Card::Card(std::string card){
    if (card[0] == 'A' || card[0] == 'a'){
        value = 1;
    } else if (card[0] == 'K' || card[0] == 'k'){
        value = 13;
    } else if (card[0] == 'Q' || card[0] == 'q'){
        value = 12;
    } else if (card[0] == 'J' || card[0] == 'j'){
        value = 11;
    } else if (card[0] == 'T' || card[0] == 't'){
        value = 10;
    } else {
        value = card[0] - '0'; // converting character to int (works since char encoded as number)
    }

    if (card[1] == 'C' || card[1] == 'c'){
        suit = Clubs;
    } else if (card[1] == 'D' || card[1] == 'd'){
        suit = Diamonds;
    } else if (card[1] == 'H' || card[1] == 'h'){
        suit = Hearts;
    } else {
        suit = Spades;
    }
}

Card::Card(int cardVal, Suit cardSuit){
    value = cardVal;
    suit = cardSuit;
}

int Card::getVal() {
    return value;
}

Suit Card::getSuit() {
    return suit;
}

std::string Card::getCardStr(){
    std::string cardStr;
    if (value == 1){ // 'A'
        cardStr += 'A';
    } else if (value < 10){ // Digit is fine
        cardStr += std::to_string(value);
    } else if (value == 10){ // 'T'
        cardStr += 'T';
    } else if (value == 11){ // 'J'
        cardStr += 'J';
    } else if (value == 12){ // 'Q'
        cardStr += 'Q';
    } else if (value == 13){ // 'K'
        cardStr += 'K';
    }

    if (suit == Clubs){
        cardStr += 'c';
    } else if (suit == Diamonds){
        cardStr += 'd';
    } else if (suit == Hearts){
        cardStr += 'h';
    } else { // Spades
        cardStr += 's';
    }

    return cardStr;
}