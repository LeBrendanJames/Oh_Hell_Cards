

#ifndef CARDS_GAMESTATE_H
#define CARDS_GAMESTATE_H

#include "../card/Card.h"
#include <iostream>
#include <random>

const int BID_CORRECT_BONUS = 10;

class GameState{
public:
	GameState(int numPlyrs, int heroPosition, int totalCards, Card * flippedCard);
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
    int getFinalScore(int plyrPosiiton);
    // getters for arrays
    Card * getCardFromPlyrHands(int player, int cardPosition);
    Card * getCardFromPlydCrds(int round, int position);
    Card * getFlippedCard();

    void setBid(int position, int bid);
    bool addCardToPlyrHand(int player, std::string card);

    bool isTrump(Card * card);
	bool makeBid(int bid);
	bool playCard(int cardPositionInHand);
    bool cardPrevUsed(std::string card);
	bool calcFinalScores();
	bool allHandsGenerated();

	void deleteAllHands();
	void deleteAllPlydCrds();
	void setNextToAct(int position);

	void reversePlay(std::string tempCard);

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
    int * finalScores;

	Card *** plyrHands; // 2d array with each row being a player and each column being a card
    Card *** plydCrds; // 2d array with each row being each successive round and each column being a player
	Card * flippedCard;

	bool checkValidPlay(int position, int cardToPlay); // Used in playCard
    bool addCardToPlydCrds(int round, int position, std::string card);
    bool removeCardFromPlydCrds(int round, int position);
    bool restoreCardToPlyrHand(int plyrPosition, std::string card);
    bool removeCardFromPlyrHand(int plyrPosition, std::string card);
    void updateNextToAct();
    void reverseNextToAct();
    int findTrickWinner(int trickNum);
};

#endif //CARDS_GAMESTATE_H