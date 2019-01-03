

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
    int getHeroPosition();
    int getTotalCards();
    int getCardsRemaining();
    Suit getTrump();
    int getNextToAct();
	int getCurrRound();
    int getBid(int position);
    int getRoundLead(int roundNum);

    void setBid(int position, int bid);

    bool isTrump(Card * card);
    void decCardsRemaining();
	void genOpponentHands();
	bool playCard(int cardToPlay);
	void addCardPlayed(std::string card);
    void removeCardFromHand(int plyrPosition, std::string card);
	void updateNextToAct();
    int findTrickWinner(int trickNum);


private:
    int numPlyrs;
    int heroPosition;
    int totalCards;
    int numCardsRemaining;
    Suit trump;
	int nextToAct;
	int currRound;

    int * bids;
	int * roundLead;
	
	Card *** plyrHands; // 2d array with each row being a player and each column being a card
    Card *** plydCrds; // 2d array with each row being each successive round and each column being a player
	Card * flippedCard;
	
	bool cardAlreadyUsed(int cardVal, int cardSuit); // Used in genOpponentHands 
	bool checkValidPlay(int position, int cardToPlay); // Used in playCard
};

#endif //CARDS_GAMESTATE_H