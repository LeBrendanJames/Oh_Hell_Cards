

#include "DecisionPoint.h"

DecisionPoint::DecisionPoint(GameState *currGmSt){
	position = gmSt->getNextToAct();
	scores = new std::vector<int>;
	for (int i = 0; i < gmSt->getNumPlyrs(); i++){
		scores->push_back(0);
	}
}

DecisionPoint::~DecisionPoint(){
	delete scores;
}


void DecisionPoint::makePlay(GameState * currGmSt){ // **MIGHT WANT TO RETURN CARD PLAYED, WHICH WILL BE IGNORED FOR ALL BUT THE TOP CALL TO MAKEPLAY
	// make tallyScores array w/ number of players & cards remaining as dimensions
	int ** tallyScores = new int*[currGmSt->getNumPlyrs()];
	for (int i = 0; i < currGmSt->getNumPlyrs(); i++){
		tallyScores[i] = new int[currGmSt->getCardsRemaining()](); // ***Must be set all to 0 to start. I think this works...***
	}
	
	GameState * newGmSt = nullptr;
	
	for (int i = 0; i < currGmSt->getCardsRemaining(); i++){
		// copy game state 
		newGmSt = new GameState(*currGmSt);
		
		// add card played
		bool validPlay = newGmSt->playCard(i); // **THIS SHOULD HAVE THE VALIDPLAY CHECK, AND WILL JUST SKIP ALOT OF THE BELOW IF IT ISNT VALID PLAY**
				// TODO: playCard needs to:
					// 1. Check if play is valid 
					// If it is:
					// 2. Add card to playedCards 2d arrays
					// 3. Remove card from players hand (and move the rest up? That's slow w/ an array so this should probably be a vector. Can then just swap to end and pop_back.)
					// 4. Call updateNextToPlay 
					// 4. return true 
					// Else return false 
		
		if (validPlay){
			// update who is next to play
			// newGmSt->updateNextToPlay(); **CALLED WITHIN PLAYCARD, ABOVE**
			
			if (newGmSt->getNextToPlay() != -1){ // recursive case 
				// Create new node & call its makePlay function
				auto * newNode = new DecisionPoint(newGmSt);
				newNode->makePlay(newGmSt);
				
				// Reach in to new node's scores array (w/ getter function?) and copy into tallyScores array
				for (int j = 0; j < currGmSt->getNumPlyrs(); i++){
					tallyScores[j][i] = newNode->getScore(j);
				}
				
				// Delete that new node here?
				delete newNode;
				newNode = nullptr;
				
			} else { // base case 
				// End of game
				tallyScores(&newGmSt, tallyScoresArray, i); // will use position member variable as well
			}
		}
		
		delete newGmSt;
		newGmSt = nullptr;
	}
	
	// set scores array to most favorable of tallyScores output arrays (based on max score in tallyScoresArray @ playNode's position)
	for (int i = 0; i < currGmSt->getCardsRemaining(); i++){
		if (tallyScores[this->position][i] > scores[this->position]){
			for (int j = 0; j < currGmSt->getNumPlyrs(); j++){
				scores[j] = tallyScores[j][i];
				cardPlayed = i; // ??????
			}
		}
	}
	
	// delete tallyScoresArray (assuming I dynamically allocate it, above)
	for (int i = 0; i < currGmSt->getNumPlyrs(); i++){
		delete [] tallyScores[i];
	}
	delete tallyScores;
	
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





