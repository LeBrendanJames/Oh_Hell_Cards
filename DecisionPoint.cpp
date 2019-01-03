

#include "DecisionPoint.h"

DecisionPoint::DecisionPoint(GameState *currGmSt){
	position = currGmSt->getNextToAct();
	scores = new std::vector<int>;
	for (int i = 0; i < currGmSt->getNumPlyrs(); i++){
		scores->push_back(0);
	}
}

DecisionPoint::~DecisionPoint(){
	delete scores;
}


Card DecisionPoint::makePlay(GameState * currGmSt){
	int cardPlayed = -1;

	// make tallyScores array w/ number of players & cards remaining as dimensions
	int ** tallyScores = new int*[currGmSt->getNumPlyrs()];
	for (int i = 0; i < currGmSt->getNumPlyrs(); i++){
		tallyScores[i] = new int[currGmSt->getCardsRemaining()](); // ***Must be set all to 0 to start. I think this works...***
	}
	
	GameState * newGmSt = nullptr;
	
	for (int i = 0; i < currGmSt->getCardsRemaining(); i++){
		// copy game state 
		newGmSt = new GameState(*currGmSt);

		// GEN OPPONENT HANDS
        newGmSt->genOpponentHands();
		
		// add card played
		bool validPlay = newGmSt->playCard(newGmSt->getRound(), i);
		
		if (validPlay){ // if validPlay, then play has been made
			
			if (newGmSt->getNextToAct() != -1){ // recursive case 
				// Create new node & call its makePlay function
				auto * newDPoint = new DecisionPoint(newGmSt);
				newDPoint->makePlay(newGmSt);
				
				// Reach in to new node's scores array (w/ getter function?) and copy into tallyScores array
				for (int j = 0; j < currGmSt->getNumPlyrs(); i++){
					tallyScores[j][i] = newDPoint->getScore(j);
				}
				
				// Delete that new node here?
				delete newDPoint;
				newDPoint = nullptr;
				
			} else { // base case 
				// End of game
				tallyScores(&newGmSt, tallyScoresArray, i); // will use position member variable as well, which is in newGmSt
			}
		}
		
		delete newGmSt;
		newGmSt = nullptr;
	}
	
	// set scores array to most favorable of tallyScores output arrays (based on max score in tallyScoresArray @ playNode's position)
	for (int i = 0; i < currGmSt->getCardsRemaining(); i++){
		if (tallyScores[this->position][i] > scores[this->position]){
			for (int j = 0; j < currGmSt->getNumPlyrs(); j++){
				scores[j] = tallyScores[j][i]; // TODO: Change '=' to '+=' when inside a loop for # of sims?
				cardPlayed = i; // TODO: move out of this loop (& # sims loop) and make it the index of scores that mazimizes scores (once theres a # sims loop)
			}
		}
	}
	
	// delete tallyScoresArray (assuming I dynamically allocate it, above)
	for (int i = 0; i < currGmSt->getNumPlyrs(); i++){
		delete [] tallyScores[i];
	}
	delete [] tallyScores;


	return *(plyrHands[currGmSt->getHeroPosition()][cardPlayed]);
}


void DecisionPoint::tallyScores(GameState * gmSt, int ** tallyScoresArray, int tallyScoreCol){
	// GameState will be full with all bids and cards played, so should be a pretty simple tally of each trick then bonuses for tricks won = bid 
	for (int i = 0; i < gmSt->getTotalCards(); i++){
		int firstPosition = gmSt->getRoundLead(i); // TODO: Need to save who leads each round (or I guess who leads the first round - all others can be figured out from that)
		
		Card currWinningCard = cardsPlayed[i][firstPosition];
		int currWinningPosition = firstPosition;
		
		int comparePosition = firstPosition + 1;
		
		for (int j = 1; j < gmSt->getNumPlyrs(); j++) { // Loop through other players, checking if their card is in the leads
			if (isTrump(currWinningCard)){
				if (isTrump(cardsPlayed[i][comparePosition])){
					// compare values
					if (cardsPlayed[i][comparePosition].value > currWinningCard.getVal()){
						// make new card winning card 
						currWinningCard = cardsPlayed[i][comparePosition];
						currWinningPosition = comparePosition;
					}
				}
			} else if (isTrump(cardsPlayed[i][comparePosition])){
				// Make new card the winning card 
				currWinningCard = cardsPlayed[i][comparePosition];
				currWinningPosition = comparePosition;
			} else if (cardsPlayed[i][comparePosition].value > currWinningCard.getVal()){
				// Make new card the winning card 
				currWinningCard = cardsPlayed[i][comparePosition];
				currWinningPosition = comparePosition;

			}
			
			comparePosition = (comparePosition + 1) % gmSt->getNumPlyrs(); // Finding next position, looping back to position 0 with the % math.
		}
		
		// Add 1 to score of winning position 
		tallyScoresArray[currWinningPosition][tallyScoreCol]++;
	}

	// check scores of each position versus their bids. Add 10 to score if they match.
	for (int i = 0; i < gmSt->getNumPlyrs(); i++){
		if (tallyScoresArray[i][tallyScoreCol] == gmSt->getBid(i)){
			tallyScoresArray[i][tallyScoreCol] += BID_CORRECT_BONUS;
		}
	}
	
}





