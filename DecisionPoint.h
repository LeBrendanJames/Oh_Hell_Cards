

#ifndef CARDS_PLAYNODE_H
#define CARDS_PLAYNODE_H


class DecisionPoint{
public:
	DecisionPoint(GameState * currGmSt);
	~DecisionPoint();
	
	Card makePlay(GameState * currGmSt);
	

private:
	int position;
	std::vector<int> * scores;
	
	void tallyScores(GameState * gmSt, int ** tallyScoresArray, int tallyScoreCol);
};

#endif //CARDS_PLAYNODE_H