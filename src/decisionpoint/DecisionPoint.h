

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
//const double PLAY_CONFIDENCE = 0.1;

class DecisionPoint{
public:
	DecisionPoint(GameState * currGmSt);
	~DecisionPoint();

	int getScore(int index);

	int recommendBid();
	bool genOpponentHands();
	Card* recommendPlay();
	Card* makePlay();
	bool isTie();

private:
	int position;
	std::vector<int> scores;
	GameState * gmSt;
	bool tie;

	bool statSignificantResult(int * optimalBidCount);
    bool statSignificantPlay(int * optimalPlayCount);
	int findSecondLargest(int * optimalBidCount);
    int findSecondLargestPlay(int * optimalPlayCount);
	int findBestBid();
	void markInvalidSuits(int position, bool * validSuits);
	bool isValidSuit(Card * card, bool * validSuits);
	void addRandomHand(GameState * indivGmSt, int position);
	void randomizeHandOrder();
	Card* makePlayRecurse();
};

#endif //CARDS_DECISIONPOINT_H