

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

int Card::getVal() {
    return value;
}

Suit Card::getSuit() {
    return suit;
}