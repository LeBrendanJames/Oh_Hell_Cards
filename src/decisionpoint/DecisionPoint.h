

#ifndef CARDS_DECISIONPOINT_H
#define CARDS_DECISIONPOINT_H

#include <iostream>
#include <vector>
#include "../gamestate/GameState.h"
#include "../card/Card.h"

class DecisionPoint{
public:
	DecisionPoint(GameState * currGmSt);
	~DecisionPoint();

	int getScore(int index);

	int makeBid();
	bool genOpponentHands();
	Card* makePlay();

private:
	int position;
	std::vector<int> scores;
	GameState * gmSt;
	
	void markInvalidSuits(int position, bool * validSuits);
	bool isValidSuit(Card * card, bool * validSuits);
	void addRandomHand(GameState * indivGmSt, int position);
};

#endif //CARDS_DECISIONPOINT_H