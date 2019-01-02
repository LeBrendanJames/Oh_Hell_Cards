

#ifndef CARDS_PLAYNODE_H
#define CARDS_PLAYNODE_H


class PlayNode{
public:
	PlayNode(GameState * currGmSt);
	~PlayNode();
	
	void makePlay(GameState * currGmSt);
	

private:
	int position;
	std::vector<int> * scores;
	
	void tallyScores(GameState * gmSt, int ** tallyScoresArray, int tallyScoreCol);
};

#endif //CARDS_PLAYNODE_H
