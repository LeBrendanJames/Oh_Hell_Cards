

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


int DecisionPoint::makeBid(){
	int optimalBid = -1;

    if (!gmSt->allHandsGenerated()) {
        //std::cout << "Calling genOpponentHands() from makeBid()" << std::endl;
        bool handsGenerated = genOpponentHands();
        if (!handsGenerated){
            return -1;
        }
    }
	
	// for bid = 0 to totalCards
	for (int i  = 0; i <= gmSt->getTotalCards(); i++){
		// copy gameState
		GameState * newGmSt = new GameState(*gmSt);
		
		DecisionPoint * newDPoint = nullptr;
		
		if (gmSt->getBid(gmSt->getNextToAct()) == -1) {
			// copiedGmSt->makeBid(i) // **THIS IS THE EQUIVALEND OF NEWGMST->PLAYCARD() IN MAKEPLAY()
			newGmSt->makeBid(i);
			
			// make newDecisionPoint with copied gameState
            // Note: New DecisionPoint must be made after newGmSt has been updated with the bid
            // so that it is simulating from the next player to act
			newDPoint = new DecisionPoint(newGmSt);

			// newDPoint->makeBid()
			newDPoint->makeBid();
			
		} else {
			// make newDecisionPoint with copied gameState 
			newDPoint = new DecisionPoint(newGmSt);
			
			// newDecisionPoint->makePlay()
			Card * playReturn = newDPoint->makePlay(); // playCard (the equivalent of makeBid, above) happens within this function & updates newGmSt
			//if (playReturn == nullptr){
			//    return -1;
			//}
		}
		
		// Look @ scores here?
		if (newDPoint->getScore(this->position) > scores[this->position]){
			optimalBid = i;
			/*
			// Update cardPlayed & scores array
			if (cardPlayed != nullptr){
				delete cardPlayed;
				cardPlayed = nullptr;
			}
			cardPlayed = new Card(tempCardPlayed->getCardStr());
			*/
			for (int j = 0; j < gmSt->getNumPlyrs(); j++){
				scores[j] = newDPoint->getScore(j);
			}
		}
		
		// Delete that new DecisionPoint here
		delete newDPoint;
		newDPoint = nullptr;
		
	}
		
	
	return optimalBid;
}

/*
// TODO: More advanced card generation will take into account a player's bidirectional_iterator
// For example, there is only some subset of hands that will optimally bid 0, so make sure
// that the card generation algorithm generates one of that set of hands if the player has bid 0.
void DecisionPoint::genOpponentHands() {
	bool validSuits[4] {true};
	bool validSuit = false;

	int cardVal = -1, cardSuit = -1;
	bool cardPrevUsed = true;

	for (int i = 0; i < gmSt->getNumPlyrs(); i++){
		if (i != gmSt->getHeroPosition()){
			for (int j = 0; j < 4; j++){
				validSuits[j] = true;
			}
			markInvalidSuits(i, validSuits);
			
			for (int j = 0; j < gmSt->getCardsRemaining(); j++){
				// Generate random card, making sure it hasn't already been used & its suit is valid (based on what player has previously played)
				do {
					cardVal = rand() % 13 + 1;
					cardSuit = rand() % 4 + 1;
					Card * tempCard = new Card(cardVal, cardSuit);

					cardPrevUsed = gmSt->cardPrevUsed(tempCard->getCardStr());
					validSuit = isValidSuit(tempCard, validSuits);

					delete tempCard;
					tempCard = nullptr;
				} while (cardPrevUsed || !validSuit);

				// Add to plyrHands array
                Card * cardToAdd = new Card(cardVal, cardSuit);
                // If player is in earlier position than hero,
                // then their 1st card has already been played,
                // so skip adding that card to hand
                if (!(j == 0 && i < gmSt->getHeroPosition() && gmSt->getCardFromPlydCrds(0, 0) != nullptr)) {
                    gmSt->addCardToPlyrHand(i, cardToAdd->getCardStr());
                }
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
	        if (gmSt->getCardFromPlyrHands(i, j) != nullptr) {
                std::cout << " " << gmSt->getCardFromPlyrHands(i, j)->getCardStr();
            } else {
	            std::cout << " " << "--";
	        }
	    }
	    std::cout << std::endl;
	}


}
*/

// Called within the first call to makePlay, so every player has a bid
// and potentially some players have already played a card in the first round
bool DecisionPoint::genOpponentHands(){
    //std::cout << "In genOpponentHands" << std::endl;
    // Make a copy of gameState
        // To hold randomly generated hands that match bids (for all players, including hero)
        // So, this copied gameState holds bids for everyone, but no hands yet
    GameState * masterGmSt = new GameState(gmSt->getNumPlyrs(), gmSt->getHeroPosition(),
                                           gmSt->getTotalCards(), gmSt->getFlippedCard());
    // copy over bids
    for (int i = 0; i < masterGmSt->getNumPlyrs(); i++){
        masterGmSt->setBid(i, gmSt->getBid(i));
    }
    // **Maybe need to copy over plydCrds as well for when making recommendation halfway through game?**


    //std::cout << "Made masterGmSt" << std::endl;

    //**Also think I need an 'excludedCards' array that I can use in below loop
    // so that I don't randomly generate a hand that overlaps with hero's hand from the gmSt
    // that calls genOpponentHands()**

    // For each player
    for (int i = 0; i < gmSt->getNumPlyrs(); i++) {
        GameState * indivGmSt = nullptr;
        DecisionPoint * newDPoint = nullptr;

        int noMatchCount = 0;

        // do
        do {
            //std::cout << "Bids don't match. Back at top of do while loop" << std::endl;
            noMatchCount++;

            delete indivGmSt;
            indivGmSt = nullptr;
            delete newDPoint;
            newDPoint = nullptr;

            // Make a copy of masterGameState, delete bids for player in question + any acting after player
            indivGmSt = new GameState(*masterGmSt);
            // put random hands in for everyone from player in question forward
                // Important to put hands in for everyone so that when calling makeBid() we don't loop back into
                // genOpponentHands recursively
                // **make use of 'excludedCards' to make sure I'm not generating hands that contain
                // cards that are in heroHand in gmSt**
            for (int j = i; j < indivGmSt->getNumPlyrs(); j++) {
                addRandomHand(indivGmSt, j); // Adds hand that doesn't have cards previously used in indivGmSt or gmSt
            }

            //std::cout << "Added random hands for everyone. Hands: " << std::endl;
            //for (int k = 0; k < indivGmSt->getNumPlyrs(); k++){
            //    std::cout << "Player #" << k;
            //    for (int j = 0; j < indivGmSt->getCardsRemaining(); j++){
            //        if (indivGmSt->getCardFromPlyrHands(k, j) != nullptr) {
            //            std::cout << " " << indivGmSt->getCardFromPlyrHands(k, j)->getCardStr();
            //        } else {
            //            std::cout << " " << "--";
            //        }
            //    }
            //    std::cout << std::endl;
            //}

            // Make sure that I've got gameState set on the correct player next to act before creating DecisionPoint
            for (int j = 0; j < masterGmSt->getNumPlyrs(); j++){
                indivGmSt->setBid(j, -1);
            }
            for (int j = 0; j < i; j++){ //TODO: I think this can be moved in previous loop within an if (j < i) stmt
                indivGmSt->makeBid(masterGmSt->getBid(j)); // Sets bid in indivGmSt corrrectly and updates nextToAct
            }

            //std::cout << "Set bids for players in front of position I'm testing bid for." << std::endl;

            // Make a decisionPoint with that and check that the bid from that player would be correct
            newDPoint = new DecisionPoint(indivGmSt);

            //std::cout << "Made new DecisionPoint" << std::endl;
            //std::cout << "Bid coming out of that DecisionPoint: " << newDPoint->makeBid() << std::endl;
            //std::cout << "Desired bid (coming from masterGmSt): " << masterGmSt->getBid(i) << std::endl;

        // while (bid from decisionPoint doesn't match actual bid)
        } while (newDPoint->makeBid() != masterGmSt->getBid(i) && masterGmSt->getBid(i) != -1 && noMatchCount < 300);

        if (noMatchCount >= 300){
            //std::cout << "Returning false from genOpponentHands because bid not possible" << std::endl;
            return false;
        }

        //std::cout << "Out of do while loop, meaning I have a hand that matches bids" << std::endl;

        delete newDPoint;
        newDPoint = nullptr;

        // Take that generated hand and place it in the first copied gameState (at top of function)
            // Still need to do the above for hero so that positions after can look at hero's hand as random,
            // but actual game will obviously have just normal hero's hand
        for (int j = 0; j < masterGmSt->getCardsRemaining(); j++){
            masterGmSt->addCardToPlyrHand(i, indivGmSt->getCardFromPlyrHands(i, j)->getCardStr());
        }

        //std::cout << "Copied that hand to masterGmSt" << std::endl;

        delete indivGmSt;
        indivGmSt = nullptr;
    }

    // Take that first copy of gameState and copy everyone except hero's hands over to the actual gameState contained in
    // the calling DecisionPoint
    for (int i = 0; i < gmSt->getNumPlyrs(); i++){
        if (i != gmSt->getHeroPosition()){
            for (int j = 0; j < gmSt->getCardsRemaining(); j++){
                gmSt->addCardToPlyrHand(i, masterGmSt->getCardFromPlyrHands(i, j)->getCardStr());
            }
        }
    }

    delete masterGmSt;
    masterGmSt = nullptr;

    // returning
    // Only change after this function has been run is that gmSt member variable has all player hands filled
    // with cards that would have bid numbers that match what they actually bid in the game

    //std::cout << "About to return. Cards copied into gmSt: " << std::endl;
    //for (int i = 0; i < gmSt->getNumPlyrs(); i++){
	//    std::cout << "Player #" << i;
	//    for (int j = 0; j < gmSt->getCardsRemaining(); j++){
	//        if (gmSt->getCardFromPlyrHands(i, j) != nullptr) {
    //            std::cout << " " << gmSt->getCardFromPlyrHands(i, j)->getCardStr();
    //        } else {
	//            std::cout << " " << "--";
	//        }
	//    }
	//    std::cout << std::endl;
	//}

    return true;

}

void DecisionPoint::addRandomHand(GameState * indivGmSt, int position){
    bool validSuits[4] {true};
    bool validSuit = false;
    int cardVal = -1, cardSuit = -1;
    bool cardPrevUsed = true;

    for (int j = 0; j < 4; j++){ // 4 = 4 suits, so fine to have in there as a constant
        validSuits[j] = true;
    }
    markInvalidSuits(position, validSuits);


    for (int j = 0; j < gmSt->getCardsRemaining(); j++){
        // Generate random card, making sure it hasn't already been used & its suit is valid (based on what player has previously played)
        do {
            cardVal = rand() % 13 + 1;
            cardSuit = rand() % 4 + 1;
            Card * tempCard = new Card(cardVal, cardSuit);

            // Checks if card was previouosly used in gmSt or indivGmSt
            cardPrevUsed = (gmSt->cardPrevUsed(tempCard->getCardStr()) ||
                            indivGmSt->cardPrevUsed(tempCard->getCardStr()));
            validSuit = isValidSuit(tempCard, validSuits);

            delete tempCard;
            tempCard = nullptr;
        } while (cardPrevUsed || !validSuit);

        // Add to plyrHands array
        Card * cardToAdd = new Card(cardVal, cardSuit);
        indivGmSt->addCardToPlyrHand(position, cardToAdd->getCardStr());

        delete cardToAdd;
        cardToAdd = nullptr;
    }
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

	if (!gmSt->allHandsGenerated()){
	    bool handsGenerated = genOpponentHands();

	    if (!handsGenerated){
	        return nullptr;
	    }

	    /*
	    std::cout << "Bids are: " << std::endl;
	    for (int i = 0; i < gmSt->getNumPlyrs(); i++){
	        std::cout << "Player #" << i << ": " << gmSt->getBid(i) << std::endl;
	    }
	    std::cout << "Hands are: " << std::endl;
        for (int i = 0; i < gmSt->getNumPlyrs(); i++){
            std::cout << "Player #" << i;
            for (int j = 0; j < gmSt->getCardsRemaining(); j++){
                if (gmSt->getCardFromPlyrHands(i, j) != nullptr) {
                    std::cout << " " << gmSt->getCardFromPlyrHands(i, j)->getCardStr();
                } else {
                    std::cout << " " << "--";
                }
            }
            std::cout << std::endl;
        }
        */
	}

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
				} else if (newDPoint->getScore(this->position) == scores[this->position]){
				    // If the scores are equal, then playing either card is fine, so I randomize which
                    // card is played
                    // Otherwise, by choosing one over the other every time it seems to higher up
                    // function that the one card is in fact better than the other
				    int random = rand() % 2;
				    if (random == 1){
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

	//std::cout << "Returning " << cardPlayed->getCardStr() << " from makePlay()" << std::endl;
    return cardPlayed;
}