

#include "Card.h"


Card::Card(std::string card){
	// 1st character is value. Convert letter value to number or, if 2-9, convert from char directly to int
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

	// 2nd character is suit. Convert to Suit ENUM.
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

Card::Card(int cardVal, int cardSuit){
	value = cardVal;
	
	if (cardSuit == 1){
		suit = Clubs;
	} else if (cardSuit == 2){
		suit = Diamonds;
	} else if (cardSuit == 3){
		suit = Hearts;
	} else { // (cardSuit == 4)
		suit = Spades;
	}
}

// Copy constructor
Card::Card(const Card &oldCard){
    this->value = oldCard.value;
    this->suit = oldCard.suit;
}

// Operator== matches value and suit 
bool operator== (Card const &rhCard){
	if (value = rhCard.value && suit = rhCard.suit){
		return true;
	}
	
	return false;
}

// Ace saved with value of 1, but is 'greater' than all other values
bool Card::operator< (Card const &rhCard){
    if (value == 1){ // lhCard is Ace
        return false;
    } else if (rhCard.value == 1){ // rhCard Ace, lhCard not
        return true;
    } else {
        return value < rhCard.value;
    }
}

// Ace saved with value of 1, but is 'greater' than all other values
bool Card::operator> (Card const &rhCard){
    if (rhCard.value == 1){
        return false;
    } else if (value == 1){
        return true;
    } else {
        return value > rhCard.value;
    }
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
    } else if (value < 10){ // Digit is fine (2 - 9)
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