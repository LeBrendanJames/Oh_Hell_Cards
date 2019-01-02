

#ifndef CARDS_CARD_H
#define CARDS_CARD_H

#include <string>
#include <iostream>

enum Suit {Clubs, Diamonds, Hearts, Spades};


class Card {
public:
    Card(std::string card);
    Card(int cardVal, Suit cardSuit);
    Card(const Card &oldCard);
    ~Card() = default;

    // TODO: Add comparison operator so that Ace (value 1) greater than everything else
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