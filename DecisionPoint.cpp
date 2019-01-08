

#include "DecisionPoint.h"

DecisionPoint::DecisionPoint(GameState *currGmSt){
	position = currGmSt->getNextToAct();

	for (int i = 0; i < currGmSt->getNumPlyrs(); i++){
		scores.push_back(-1);
	}

	gmSt = new GameState(*currGmSt); // Gamestate copied when decisionPoint constructed

    cardPlayed = nullptr;
}

DecisionPoint::~DecisionPoint(){
	delete gmSt;
	delete cardPlayed;
}

int DecisionPoint::getScore(int index){
    if (index >= 0 && index < scores.size()) {
        return scores[index];
    } else {
        return -1;
    }
}


void DecisionPoint::genOpponentHands() {
    // TODO: Gen opponent hands needs to take into account the cards a player has already played
    // For example, if Hearts is led and they didn't follow Hearts in a previous round, then they can't have hearts

	int cardVal = -1, cardSuit = -1;
	bool cardPrevUsed = true;

	for (int i = 0; i < gmSt->getNumPlyrs(); i++){
		if (i != gmSt->getHeroPosition()){
			for (int j = 0; j < gmSt->getCardsRemaining(); j++){
				// Generate random card, making sure it hasn't already been used
				do {
					cardVal = rand() % 13 + 1;
					cardSuit = rand() % 4 + 1;
					Card * tempCard = new Card(cardVal, cardSuit);

					cardPrevUsed = gmSt->cardPrevUsed(tempCard->getCardStr());

					delete tempCard;
					tempCard = nullptr;
				} while (cardPrevUsed);

				// Add to plyrHands array
                Card * cardToAdd = new Card(cardVal, cardSuit);
                gmSt->addCardToPlyrHand(i, cardToAdd->getCardStr());
                delete cardToAdd;
                cardToAdd = nullptr;
			}
		}
	}

	// Print for testing
    for (int i = 0; i < gmSt->getNumPlyrs(); i++){
	    std::cout << "Player #" << i;
	    for (int j = 0; j < gmSt->getCardsRemaining(); j++){
	        std::cout << " " << gmSt->getCardFromPlyrHands(i, j)->getCardStr();
	    }
	    std::cout << std::endl;
	}

}

Card* DecisionPoint::makePlay(){

	// make tallyScores array w/ number of players & cards remaining as dimensions
	//int ** tallyScoresArray = new int*[gmSt->getNumPlyrs()];
	//for (int i = 0; i < gmSt->getNumPlyrs(); i++){
	//	tallyScoresArray[i] = new int[gmSt->getTotalCards()]{0}; //***Must be set all to 0 to start. I think this works...***
	//}

	//std::cout << "Made tallyScoresArray" << std::endl;
	
	GameState * newGmSt = nullptr;

	// Loop through all potential cards available
    int numLoops = gmSt->getCardsRemaining();
	for (int i = 0; i < numLoops; i++){
	    std::cout << "Start of makePlay() loop. i = " << i << std::endl;
		// copy game state
        std::cout << "Copying gmSt to newGmSt" << std::endl;
		newGmSt = new GameState(*gmSt);

		std::cout << "Creating tempCardPlayed" << std::endl;
		Card * tempCardPlayed = new Card(gmSt->getCardFromPlyrHands(gmSt->getNextToAct(), i)->getCardStr());
		std::cout << "Testing card " << tempCardPlayed->getCardStr() << std::endl;

		std::cout << "About to playCard(i), i = " << i << std::endl;
		// add card played
		bool validPlay = newGmSt->playCard(i);

		std::cout << "After playCard(i), i = " << i << std::endl;
		
		if (validPlay){ // if validPlay, then play has been made
			
			if (newGmSt->getNextToAct() != -1){ // recursive case 
				// Create new node & call its makePlay function
                std::cout << "Making new DecisionPoint" << std::endl;
				auto * newDPoint = new DecisionPoint(newGmSt);

				std::cout << "About to recursively call makePlay()." << std::endl;
				newDPoint->makePlay();

				std::cout << "Out of makePlay()." << std::endl;
				std::cout << "Current best score: " << scores[this->position] << std::endl;
				std::cout << "Score from new cardPlayed: " << newDPoint->getScore(this->position) << std::endl;
				if (newDPoint->getScore(this->position) > scores[this->position]){
				    std::cout << "New score better, so copying into scores array." << std::endl;
				    // Update cardPlayed & scores array
                    if (cardPlayed != nullptr){
                        delete cardPlayed;
                        cardPlayed = nullptr;
                    }
                    cardPlayed = new Card(tempCardPlayed->getCardStr());
                    std::cout << "New card played = " << cardPlayed->getCardStr() << std::endl;
                    for (int j = 0; j < gmSt->getNumPlyrs(); j++){
                        scores[j] = newDPoint->getScore(j);
                    }
				}
				
				// Reach in to new node's scores array (w/ getter function?) and copy into tallyScores array
				//for (int j = 0; j < gmSt->getNumPlyrs(); j++){
                //    tallyScoresArray[j][i] = newDPoint->getScore(j);
				//}
				
				// Delete that new node here?
				delete newDPoint;
				newDPoint = nullptr;
				
			} else { // base case
			    std::cout << "In else stmt - bottoming out recursion" << std::endl;
			    //std::cout << "Setting cardPlayed" << std::endl;
			    cardPlayed = new Card(tempCardPlayed->getCardStr());
			    //std::cout << "cardPlayed = " << cardPlayed->getCardStr() << std::endl;
				// End of game
                //std::cout << "Calculating final scores within gmSt" << std::endl;
                newGmSt->calcFinalScores();
                //std::cout << "Copying scores over to DecisionPoint" << std::endl;
                for (int j = 0; j < gmSt->getNumPlyrs(); j++){
                    //std::cout << "scores[j] (before copy) = " << scores[j] << std::endl;
                    //std::cout << "finalScore[j] (from newGmSt) = " << newGmSt->getFinalScore(j) << std::endl;
                    scores[j] = newGmSt->getFinalScore(j); // copy scores from gmSt up to DecisionPoint
                    //std::cout << "scores[j] (after copy) = " << scores[j] << std::endl;
                }
                //std::cout << "Through copying scores." << std::endl;
			}
		}

		//std::cout << "Deleting tempCardPlayed" << std::endl;
		delete tempCardPlayed;
		tempCardPlayed = nullptr;

		//std::cout << "Deleting newGmSt" << std::endl;
		delete newGmSt;
		//std::cout << "Deleted newGmSt - looping to top" << std::endl;
		newGmSt = nullptr;
	}
	
	// set scores array to most favorable of tallyScores output arrays (based on max score in tallyScoresArray @ playNode's position)
	//for (int i = 0; i < gmSt->getCardsRemaining(); i++){
	//	if (tallyScoresArray[this->position][i] >= (*scores)[this->position]){
    //        cardPlayed = i; // TODO: move out of this loop (& # sims loop) and make it the index of scores that mazimizes scores (once theres a # sims loop)
	//		for (int j = 0; j < gmSt->getNumPlyrs(); j++){
	//			(*scores)[j] = tallyScoresArray[j][i]; // TODO: Change '=' to '+=' when inside a loop for # of sims?
	//		}
	//	}
	//}
	
	// delete tallyScoresArray (assuming I dynamically allocate it, above)
	//for (int i = 0; i < gmSt->getNumPlyrs(); i++){
	//	delete [] tallyScoresArray[i];
	//}
	//delete [] tallyScoresArray;

    std::cout << "Down to return stmt" << std::endl;
	if (cardPlayed == nullptr){
	    std::cout << "cardPlayed == nullptr" << std::endl;
	}
    std::cout << "Returning card: " << cardPlayed->getCardStr() << std::endl;
    return cardPlayed;
	//return *(gmSt->getCardFromPlyrHands(gmSt->getNextToAct(), cardPlayed));
}


