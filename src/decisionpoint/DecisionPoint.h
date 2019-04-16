

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

	int getScore(int index);
    bool isTie();

	int recommendBid();
	Card* recommendPlay();

private:
	int position;
	std::vector<int> scores;
	GameState * gmSt;
	bool tie;

    bool genOpponentHands();
    void simulateBid(int bid, int* simulationScores);
    void replaceScores(int * simulationScores);
	bool statSignificantResult(int * optimalBidCount, int size);
	int secondLargestElement(int * optimalCountArr, int size);
    void runPlaySim(int * optimalPlayCount, int& runCount, int& simCount);
	int findBestBid();
    Card* findBestPlay();
	void markInvalidSuits(int position, bool * validSuits);
	bool isValidSuit(Card * card, bool * validSuits);
	void addRandomHand(GameState * indivGmSt, int position);
	Card* makePlay();
};

#endif //CARDS_DECISIONPOINT_H