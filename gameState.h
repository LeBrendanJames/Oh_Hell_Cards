

#ifndef CARDS_GAMESTATE_H
#define CARDS_GAMESTATE_H

#include "Card.h"
#include <iostream>


class GameState{
public:
    GameState(int numPlyrs, int heroPosition, int totalCards, Card * flippedCard, Card ** heroHand);
    GameState(const GameState &oldGmSt);
    ~GameState();

    int getNumPlyrs();
    int getBid(int position);
    int getTotalCards();
    int getCardsRemaining();
    Suit getTrump();
    int getNextToAct();
    int getRoundLead(int roundNum);
    int getHeroPosition();

    void setBid(int position, int bid);

    bool isTrump(Card * card);
    void decCardsRemaining();
	void genOpponentHands(); // ?
	bool playCard(int cardToPlay);
	int findTrickWinner(int trickNum);

	void addCardPlayed(std::string card);
	void updateNextToAct();

	void removeCardFromHand(int plyrPosition, std::string card);

private:
    int numPlyrs;
    int * bids;
    int totalCards;
    int numCardsRemaining;
    Suit trump;
	int nextToAct;
	int * roundLead;
	int heroPosition;
	
	Card *** plyrHands;
    Card *** plydCrds; // 2d array with each row being each successive round and each column being a player
	Card * flippedCard;
};

#endif //CARDS_GAMESTATE_H