

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

	// Helpers to loop through random games until result is returned
    bool statSignificantResult(int * optimalBidCount, int size);
    int secondLargestElement(int * optimalCountArr, int size);

	// Finding best bid to make
    int findBestBid();
    void simulateBid(int bid, int* simulationScores);
    void replaceScores(int * simulationScores);

    // Finding best play to make
    void simulatePlay(int * optimalPlayCount);
    Card* findBestPlay();
    Card* makePlay();

    // Generating opponent hands for game simulation purposes
    bool genOpponentHands();
    bool cardAlreadyUsed(GameState * gmSt, GameState * indivGmSt, GameState * masterGmSt, int cardVal, int cardSuit);
    GameState* reconstructGmStFromStart();
    void copyOpponentHandsToGmSt(GameState * masterGmSt);
    GameState* setNewRandomHandGmSt(int currPosition, GameState * masterGmSt);
    void fillPlayerHand(GameState* masterGmSt, int plyrPosition);
    void addHandToMatchBid(GameState * masterGmSt, int plyrPosition);
    void markInvalidSuits(int position, bool * validSuits);
    bool isValidSuit(int cardSuit, bool * validSuits);
    void addRandomHand(GameState * indivGmSt, GameState * masterGmSt, int position);
};

#endif //CARDS_DECISIONPOINT_H