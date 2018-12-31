

#ifndef CARDS_GAMESTATE_H
#define CARDS_GAMESTATE_H

#include "Card.h"

class GameState{
public:
    GameState(int numPlyrs, int totalCards, Suit trump);
    GameState(const GameState &oldGmSt);
    ~GameState();

    int getNumPlyrs();
    int getBid(int position);
    int getTotalCards();
    int getCardsRemaining();
    Suit getTrump();

    void setBid(int position, int bid);

    bool isTrump(Card * card);
    void decCardsRemaining();

private:
    int numPlyrs;
    int * bids;
    int totalCards;
    int numCardsRemaining;
    Suit trump;

    Card ** prevPlayedCards; // 2d array with each row being each successive round and each column bing a player
};

#endif //CARDS_GAMESTATE_H