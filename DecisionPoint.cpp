

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

			newDPoint->makeBid();
			
		} else {
			// make newDecisionPoint with copied gameState 
			newDPoint = new DecisionPoint(newGmSt);
			
			// newDecisionPoint->makePlay()
			std::cout << "About to call makePlay()" << std::endl;
			newDPoint->makePlay(); // playCard (the equivalent of makeBid, above) happens within this function & updates newGmSt 
			
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


// TODO: More advanced card generation will take into account a player's bidirectional_iterator
// For example, there is only some subset of hands that will optimally bid 0, so make sure
// that the card generation algorithm generates one of that set of hands if the player has bid 0.
void DecisionPoint::genOpponentHands() {
    //std::cout << "In genOpponentHands" << std::endl;
	bool validSuits[4];
	std::string newCard;

	Card ** cardsToAdd = nullptr;

	// **could just gen random hands for all positions and rotate any opposing player hands
    // until we have 100 samples of all players previous to hero bidding the correct amount.
    // Then, check that player in question bid correctly on at least 70 of those**

	for (int i = 0; i < gmSt->getNumPlyrs(); i++){
		if (i != gmSt->getHeroPosition()){
		    // while !validHandForPosThatBidsCorrectly
            bool validGeneratedHand = false;
            while (!validGeneratedHand) {
                // copy gameState
                GameState * newGameState = new GameState(*gmSt);
                // gen random hand for position in question (checking that it would satisfy actual gameState)
                genHand(validSuits, cardsToAdd);
                // add cardsToAdd to new gameState
                
                // change View
                newGameState->chgPlyrView(i, newPlyrHand);
                int correctPrevBids = 0;
                // while count correctPrevBids < 100
                while (correctPrevBids < 100) {
                    // gen random hands for everybody (except position in question)
                    // run simulation
                    // check whether previous bids were correct and tally whether player in question optimal bid is correct
                    bidMatch = true;
                    for (int j = 0; j < i; j++){
                        if (gmSt->getBid(j) != newGameState->getBid(j)){
                            bidMatch = false;
                        }
                    }
                    if (bidMatch){
                        correctPrevBids++;
                    }
                }
                // Copy out hand in question if it'll work
                if (correctPlyrBid > correctPrevBids * 0.7){
                    // copy out hand
                    validGeneratedHand = true;
                }
                // delete copied gameState
                delete newGameState;
                newGameState = nullptr;
            }

            // put hand in question into actual gameState





		    // Set valid/invalid suits
			for (int j = 0; j < 4; j++){
				validSuits[j] = true;
			}
			markInvalidSuits(i, validSuits);

			// Allocats space for cardsToAdd
            cardsToAdd = new Card*[gmSt->getCardsRemaining()];

            // Fill cardsToAdd based on vlaidSuits and maybe plyr bid
			if (gmSt->getBid(i) != -1){
			    //std::cout << "Calling genHandCondtlOnBid" << std::endl;
			    genHandCondtlOnBid(validSuits, cardsToAdd, i);
			} else {
			    //std::cout << "Calling genHand" << std::endl;
			    genHand(validSuits, cardsToAdd);
			}

            // Add cardsToAdd to gmSt and delete cardsToAdd memory
            for (int j = 0; j < gmSt->getCardsRemaining(); j++){
			    gmSt->addCardToPlyrHand(j, cardsToAdd[j]->getCardStr());
			    delete cardsToAdd[j];
			}
			delete cardsToAdd;
			cardsToAdd = nullptr;

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

void DecisionPoint::genHandCondtlOnBid(bool * validSuits, Card ** cardsToAdd, int plyrPos){
    DecisionPoint * testDPoint = nullptr;

    GameState * testGmSt = new GameState(*gmSt);

    //std::cout << "In genHandCondtlOnBid. Trying to get bid of " << gmSt->getBid(plyrPos) << std::endl;

    do {
        //std::cout << "About to generate a random hand" << std::endl;
        genHand(validSuits, cardsToAdd);

        //std::cout << "Generated hand. 1st Card: " << cardsToAdd[0]->getCardStr() << std::endl;

        testGmSt->chgPlyrView(plyrPos, cardsToAdd);

        delete testDPoint;
        testDPoint = nullptr;
        testDPoint = new DecisionPoint(testGmSt);

        std::cout << "About to call testDPoint->makeBid()" << std::endl;
        int bidRec = testDPoint->makeBid();
        std::cout << "bidRec: " << bidRec << std::endl;

    } while (bidRec != gmSt->getBid(plyrPos));
    //} while (testDPoint->makeBid() != gmSt->getBid(plyrPos));

    std::cout << "Out of do while loop in genHandCondtlOnBid" << std::endl;

    delete testGmSt;
    testGmSt = nullptr;
    delete testDPoint;
    testDPoint = nullptr;
}

void DecisionPoint::genHand(bool * validSuits, Card ** cardsToAdd){
    bool match = false;
    std::string newCard;

    for (int i = 0; i < gmSt->getCardsRemaining(); i++){
        do {
            match = false;

            // Checks against all cards in gmSt
            newCard = genRandomCard(validSuits);

            // Check against random cards already generated for hand (not yet in gmSt)
            for (int j = 0; j < i; j++){
                if (newCard == cardsToAdd[j]->getCardStr()){
                    match = true;
                    break;
                }
            }
        } while (match);

        cardsToAdd[i] = new Card(newCard);
    }
}

std::string DecisionPoint::genRandomCard(bool * validSuits){
    std::string randCard;
    int cardVal = -1, cardSuit = -1;
    bool cardPrevUsed = true, validSuit = true;

    // Generate random card, making sure it hasn't already been used
    // & its suit is valid (based on what player has previously played)
    do {
        cardVal = rand() % 13 + 1;
        cardSuit = rand() % 4 + 1;
        Card *tempCard = new Card(cardVal, cardSuit);

        cardPrevUsed = gmSt->cardPrevUsed(tempCard->getCardStr());
        validSuit = isValidSuit(tempCard, validSuits);

        randCard = tempCard->getCardStr();

        delete tempCard;
        tempCard = nullptr;
    } while (cardPrevUsed || !validSuit);

    return randCard;
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

	// Check whether hands have been generated
    //std::cout << "Checking allHandsFull()" << std::endl;
    if (!gmSt->allHandsFull()){
        //std::cout << "allHandsFull returned false. Generating hands" << std::endl;
        // run genHands
        genOpponentHands();
        //std::cout << "Done with genHands" << std::endl;
    } else {
        std::cout << "allHandsFull returned true" << std::endl;
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