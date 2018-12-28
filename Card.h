

#ifndef CARDS_CARD_H
#define CARDS_CARD_H

#include <string>

enum Suit {Clubs, Diamonds, Hearts, Spades};


class Card {
public:
    Card(std::string card);
    ~Card() = default;

    int getVal();
    Suit getSuit();

private:
    int value;
    Suit suit;

};

#endif //CARDS_CARD_H