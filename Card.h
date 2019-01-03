// CARD CLASS
// Card class represents a playing card, with a value and suit.
// Value and suit can be entered into constructor as string, individual numbers, or number for value + Suit ENUM for suit  
// Values and suits can be accessed from getter functions, 
// and the entire card string can be accessed with the 'getCardStr' member function

#ifndef CARDS_CARD_H
#define CARDS_CARD_H

#include <string>

enum Suit {Clubs, Diamonds, Hearts, Spades};


class Card {
public:
    Card(std::string card);
    Card(int cardVal, Suit cardSuit);
	Card(int cardVal, int cardSuit);
    Card(const Card &oldCard);
    ~Card() = default;

	bool operator== (Card const &rhCard);
    bool operator< (Card const &rhCard);
    bool operator> (Card const &rhCard);

    int getVal();
    Suit getSuit();

    std::string getCardStr();

private:
    int value;
    Suit suit;

};

#endif //CARDS_CARD_H