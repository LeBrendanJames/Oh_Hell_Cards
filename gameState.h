

#ifndef CARDS_GAMESTATE_H
#define CARDS_GAMESTATE_H

#include "Card.h"

class GameState{
public:
    GameState(int numPlyrs, int totalCards, Suit trump);
    ~GameState();

    //void addHeroHand(std::string* heroHand);

private:
    int numPlyrs;
    int * bids;
    int totalCards;
    int numCardsRemaining;
    Suit trump;

    int heroPosition;
    Card * heroHand;
};


#endif //CARDS_GAMESTATE_H