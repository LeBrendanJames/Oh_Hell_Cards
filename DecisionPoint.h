

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

	int makeBid();
	void genOpponentHands();
	Card* makePlay();

private:
	int position;
	std::vector<int> scores;
	GameState * gmSt;
	Card * cardPlayed;

    void genHandCondtlOnBid(bool * validSuits, Card ** cardsToAdd, int plyrPos);
    void genHand(bool * validSuits, Card ** cardsToAdd);
	std::string genRandomCard(bool * validSuits);
	void markInvalidSuits(int position, bool * validSuits);
	bool isValidSuit(Card * card, bool * validSuits);
};

#endif //CARDS_DECISIONPOINT_H