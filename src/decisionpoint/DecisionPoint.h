

#ifndef CARDS_DECISIONPOINT_H
#define CARDS_DECISIONPOINT_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "../gamestate/GameState.h"
#include "../card/Card.h"

const int DEFAULT_BID_SIMULATIONS = 10;
const int DEFAULT_PLAY_SIMULATIONS = 10;

class DecisionPoint{
public:
	DecisionPoint(GameState * currGmSt);
	~DecisionPoint();

	// GETTERS
	int getScore(int index);
    bool isTie();

    // DECISIONPOINT CAN RECOMMEND A BID OR PLAY
	int recommendBid();
	Card* recommendPlay();

private:
	int position;
	std::vector<double> scores;
	GameState * gmSt;
	bool tie;
	double ** bonusProb;
	double ** tempBonusProbs;

	// Helpers to loop through random games until result is returned
    bool statSignificantResult(int * optimalBidCount, int size);
    int secondLargestElement(int * optimalCountArr, int size);

	// Finding best bid to make
    int findBestBid();

    // Finding best play to make
    void findBestPlay(int * optimalPlayCount);
    Card* simulatePlay(bool bidFlag);

    // Generating opponent hands for game simulation purposes
    bool genOpponentHands();
	void fillPlayerHand(GameState* masterGmSt, int plyrPosition);
    void addHandToMatchBid(GameState * masterGmSt, int plyrPosition);
    GameState* setNewRandomHandGmSt(int currPosition, GameState * masterGmSt);
    GameState* reconstructGmStFromStart();
    void copyOpponentHandsToGmSt(GameState * masterGmSt);
    void addRandomHand(GameState * indivGmSt, GameState * masterGmSt, int position);
	bool cardAlreadyUsed(GameState * gmSt, GameState * indivGmSt, GameState * masterGmSt, int cardVal, int cardSuit);
    void markInvalidSuits(int position, bool * validSuits);
    bool isValidSuit(int cardSuit, bool * validSuits);

};

#endif //CARDS_DECISIONPOINT_H