

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
	GameState * newGmSt = nullptr;

	// Loop through all potential cards available
    int numLoops = gmSt->getCardsRemaining();
	for (int i = 0; i < numLoops; i++){

		// copy game state
		newGmSt = new GameState(*gmSt);

		Card * tempCardPlayed = new Card(gmSt->getCardFromPlyrHands(gmSt->getNextToAct(), i)->getCardStr());

		// add card played
		bool validPlay = newGmSt->playCard(i);
		
		if (validPlay){ // if validPlay, then play has been made
			
			if (newGmSt->getNextToAct() != -1){ // recursive case 
				// Create new node & call its makePlay function
				auto * newDPoint = new DecisionPoint(newGmSt);

				newDPoint->makePlay();

				if (newDPoint->getScore(this->position) > scores[this->position]){
				    // Update cardPlayed & scores array
                    if (cardPlayed != nullptr){
                        delete cardPlayed;
                        cardPlayed = nullptr;
                    }
                    cardPlayed = new Card(tempCardPlayed->getCardStr());
                    for (int j = 0; j < gmSt->getNumPlyrs(); j++){
                        scores[j] = newDPoint->getScore(j);
                    }
				}
				
				// Delete that new node here?
				delete newDPoint;
				newDPoint = nullptr;
				
			} else { // base case - end of game
			    cardPlayed = new Card(tempCardPlayed->getCardStr());

                newGmSt->calcFinalScores();

                for (int j = 0; j < gmSt->getNumPlyrs(); j++){
                    scores[j] = newGmSt->getFinalScore(j); // copy scores from gmSt up to DecisionPoint
                }
			}
		}

		delete tempCardPlayed;
		tempCardPlayed = nullptr;

		delete newGmSt;
		newGmSt = nullptr;
	}

    return cardPlayed;
}