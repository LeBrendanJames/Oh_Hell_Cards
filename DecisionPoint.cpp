

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


// TODO: More advanced card generation will take into account a player's bidirectional_iterator
// For example, there is only some subset of hands that will optimally bid 0, so make sure
// that the card generation algorithm generates one of that set of hands if the player has bid 0.
void DecisionPoint::genOpponentHands() {
	bool validSuits[4] {true};

	int cardVal = -1, cardSuit = -1;
	bool cardPrevUsed = true;

	for (int i = 0; i < gmSt->getNumPlyrs(); i++){
		if (i != gmSt->getHeroPosition()){
			for (int j = 0; j < 4; j++){
				validSuits[j] = true;
			}
			markInvalidSuits(i, &validSuits);
			
			for (int j = 0; j < gmSt->getCardsRemaining(); j++){
				// Generate random card, making sure it hasn't already been used & its suit is valid (based on what player has previously played)
				do {
					cardVal = rand() % 13 + 1;
					cardSuit = rand() % 4 + 1;
					Card * tempCard = new Card(cardVal, cardSuit);

					cardPrevUsed = gmSt->cardPrevUsed(tempCard->getCardStr());
					validSuit = isValidSuit(tempCard, invalidSuits);

					delete tempCard;
					tempCard = nullptr;
				} while (cardPrevUsed || !validSuit);

				// Add to plyrHands array
                Card * cardToAdd = new Card(cardVal, cardSuit);
                gmSt->addCardToPlyrHand(i, cardToAdd->getCardStr());
                delete cardToAdd;
                cardToAdd = nullptr;
			}
		}
	}

	// Print for testing
    /*
    for (int i = 0; i < gmSt->getNumPlyrs(); i++){
	    std::cout << "Player #" << i;
	    for (int j = 0; j < gmSt->getCardsRemaining(); j++){
	        std::cout << " " << gmSt->getCardFromPlyrHands(i, j)->getCardStr();
	    }
	    std::cout << std::endl;
	}
     */

}

// Function to mark suits that cannot be part of a player's generated random hand
// The logic is that if a suit was led on a previous round and the player did not follow suit, then they cannot have any of that suit
// So, the function checks the leading suit of each completed round, and if the player did not follow suit, 
// marks the leading suit of that round as invalid for card generation for future round simulation
void DecisionPoint::markInvalidSuits(int position, bool * validSuits){
	int round = 0;
	
	while (gmSt->getCardFromPlydCrds(round, position) != nullptr){
		// If player not leading round (if they are, then any card is valid and doesn't affect genHands)
		if (position != gmSt->getRoundLead(round)){ 
			// If leading suit != suit played by player in question
			if (gmSt->getCardFromPlydCrds(round, position)->getSuit() != gmSt->getCardFromPlydCrds(round, gmSt->getRoundLead(round))->getSuit()){
				// mark led suit as invalid, since player can't have any of that suit (since they didn't follow suit)
				validSuits[gmSt->getCardFromPlydCrds(round, gmSt->getRoundLead(round))->getSuit()] = false;
			}
		}
		round++;
	}
}

bool DecisionPoint::isValidSuit(Card * card, bool * validSuits){
	for (int i = 0; i < 4; i++){
		if (card->getSuit() == i && validSuits[i] == false){ // If card is suit && suit not valid 
			return false;
		}
	}
	
	return true;
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