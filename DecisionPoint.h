

#ifndef CARDS_DECISIONPOINT_H
#define CARDS_DECISIONPOINT_H

#include <iostream>
#include <vector>
#include "GameState.h"
#include "Card.h"

class DecisionPoint{
public:
	DecisionPoint(GameState * currGmSt);
	~DecisionPoint();

	int getScore(int index);

	void genOpponentHands();
	Card* makePlay();

private:
	int position;
	std::vector<int> scores;
	GameState * gmSt;
	Card * cardPlayed;
};

#endif //CARDS_DECISIONPOINT_H