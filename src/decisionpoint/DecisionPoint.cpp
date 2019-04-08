

#include "DecisionPoint.h"

DecisionPoint::DecisionPoint(GameState *currGmSt){
	position = currGmSt->getNextToAct();

	for (int i = 0; i < currGmSt->getNumPlyrs(); i++){
		scores.push_back(-1);
	}

	gmSt = new GameState(*currGmSt); // Gamestate copied when decisionPoint constructed
}

DecisionPoint::~DecisionPoint(){
	delete gmSt;
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
		}
		
		// Look @ scores here?
		if (newDPoint->getScore(this->position) > scores[this->position]){
			optimalBid = i;
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

// Called within the first call to makePlay, so every player has a bid
// and potentially some players have already played a card in the first round
bool DecisionPoint::genOpponentHands(){
    // Make a copy of gameState
        // To hold randomly generated hands that match bids (for all players, including hero)
        // So, this copied gameState holds bids for everyone, but no hands yet
    GameState * masterGmSt = new GameState(gmSt->getNumPlyrs(), gmSt->getHeroPosition(),
                                           gmSt->getTotalCards(), gmSt->getFlippedCard());

    // copy over bids
    for (int i = 0; i < masterGmSt->getNumPlyrs(); i++){
        masterGmSt->setBid(i, gmSt->getBid(i));
    }

    // Copy over any played cards from gmSt to masterGmSt
    for (int i = 0; i < gmSt->getTotalCards(); i++){
        for (int j = 0; j < gmSt->getNumPlyrs(); j++){
            if (gmSt->getCardFromPlydCrds(i, j) != nullptr){
                masterGmSt->addCardToPlyrHand(j, gmSt->getCardFromPlydCrds(i, j)->getCardStr());
            }
        }
    }


    // For each player, produce a hand that matches their bid and copy it in to masterGmSt
    for (int i = 0; i < gmSt->getNumPlyrs(); i++) {
        GameState * indivGmSt = nullptr;
        DecisionPoint * newDPoint = nullptr;

        int noMatchCount = 0;

        do {
            noMatchCount++;

            // Reset indivGmSt and newDPoint
            delete indivGmSt;
            indivGmSt = nullptr;
            delete newDPoint;
            newDPoint = nullptr;

            // Make a copy of masterGameState, delete bids for player in question + any acting after player
            //indivGmSt = new GameState(*masterGmSt);
            indivGmSt = new GameState(masterGmSt->getNumPlyrs(), masterGmSt->getHeroPosition(),
                                      masterGmSt->getTotalCards(), masterGmSt->getFlippedCard());
            // For all players up to and including current position in question, copy over known cards in hand
            for (int j = 0; j <= i; j++){
                for (int k = 0; k < masterGmSt->getTotalCards(); k++){
                    if (masterGmSt->getCardFromPlyrHands(j, k) != nullptr){
                        indivGmSt->addCardToPlyrHand(j, masterGmSt->getCardFromPlyrHands(j, k)->getCardStr());
                    }
                }
            }

            // put random hands in for everyone from player in question forward
                // Important to put hands in for everyone so that when calling makeBid() we don't loop back into
                // genOpponentHands recursively
                // **make use of 'excludedCards' to make sure I'm not generating hands that contain
                // cards that are in heroHand in gmSt**
            for (int j = i; j < indivGmSt->getNumPlyrs(); j++) {
                addRandomHand(indivGmSt, j); // Adds hand that doesn't have cards previously used in indivGmSt or gmSt
            }


            // Make sure that I've got gameState set on the correct player next to act before creating DecisionPoint
            for (int j = 0; j < masterGmSt->getNumPlyrs(); j++){
                indivGmSt->setBid(j, -1);
            }
            for (int j = 0; j < i; j++){ //TODO: I think this can be moved in previous loop within an if (j < i) stmt
                indivGmSt->makeBid(masterGmSt->getBid(j)); // Sets bid in indivGmSt corrrectly and updates nextToAct
            }

            // Make a decisionPoint with that and check that the bid from that player would be correct
            newDPoint = new DecisionPoint(indivGmSt);

        // while (bid from decisionPoint doesn't match actual bid)
        } while (masterGmSt->getBid(i) != -1 && newDPoint->makeBid() != masterGmSt->getBid(i) && noMatchCount < 300);


        delete newDPoint;
        newDPoint = nullptr;

        // Take that generated hand and place it in the first copied gameState (at top of function)
            // Still need to do the above for hero so that positions after can look at hero's hand as random,
            // but actual game will obviously have just normal hero's hand
        for (int j = 0; j < masterGmSt->getTotalCards(); j++){
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
            for (int j = 0; j < gmSt->getTotalCards(); j++){
                // TODO: update this copying to only copy over new cards, not ones already played
                // This copy is relying on the 'addCardToPlyrHand' function rejecting any cards that have been used
                // It's working, but it's not ideal to rely on that function's error checking
                gmSt->addCardToPlyrHand(i, masterGmSt->getCardFromPlyrHands(i, j)->getCardStr());
            }
        }
    }

    delete masterGmSt;
    masterGmSt = nullptr;

    // Only change after this function has been run is that gmSt member variable has all player hands filled
    // with cards that would have bid numbers that match what they actually bid in the game

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

    for (int j = 0; j < gmSt->getTotalCards(); j++) {
        if (indivGmSt->getCardFromPlyrHands(position, j) == nullptr) {
            // Generate random card, making sure it hasn't already been used & its suit is valid (based on what player has previously played)
            do {
                cardVal = rand() % 13 + 1;
                cardSuit = rand() % 4 + 1;
                Card *tempCard = new Card(cardVal, cardSuit);

                // Checks if card was previouosly used in gmSt or indivGmSt
                cardPrevUsed = (gmSt->cardPrevUsed(tempCard->getCardStr()) ||
                                indivGmSt->cardPrevUsed(tempCard->getCardStr()));
                validSuit = isValidSuit(tempCard, validSuits);

                delete tempCard;
                tempCard = nullptr;
            } while (cardPrevUsed || !validSuit);

            // Add to plyrHands array
            Card *cardToAdd = new Card(cardVal, cardSuit);
            indivGmSt->addCardToPlyrHand(position, cardToAdd->getCardStr());

            delete cardToAdd;
            cardToAdd = nullptr;
        }
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
    std::vector<int> tempScores;
    tempScores.reserve(gmSt->getNumPlyrs());
    for (int i = 0; i < gmSt->getNumPlyrs(); i++){
        tempScores.push_back(0);
    }
    int currPosition = gmSt->getNextToAct();
    Card * cardPlayed = nullptr;

    for (int i = 0; i < gmSt->getNumPlyrs(); i++) {
        scores[i] = -1;
    }

	if (!gmSt->allHandsGenerated()){
	    bool handsGenerated = genOpponentHands();
	    if (!handsGenerated){
	        return nullptr;
	    }
	}

    cardPlayed = new Card(gmSt->getCardFromPlyrHands(gmSt->getNextToAct(), rand() % gmSt->getCardsRemaining())->getCardStr());

	// Loop through all potential cards available
    for (int i = 0; i < gmSt->getCardsRemaining(); i++){

		Card * tempCardPlayed = new Card(*(gmSt->getCardFromPlyrHands(gmSt->getNextToAct(), i)));

		// add card played
        // If play is invalid, nothing changes about gameState
        // Otherwise, card is played and nextToAct is updated (and anything else that changes about gameState)
		//bool validPlay = gmSt->playCard(i);

		if ((gmSt->playCard(i))){ // if validPlay, which returns true, then play has been made

		    if (gmSt->getNextToAct() != -1){
                // Save scores array in tempScores befre making next play
                for (int j = 0; j < gmSt->getNumPlyrs(); j++) {
                    tempScores[j] = scores[j];
                }

                makePlay();

                if (scores[currPosition] > tempScores[currPosition]) {
                    // Update cardPlayed & scores array
                    if (cardPlayed != nullptr) {
                        delete cardPlayed;
                        cardPlayed = nullptr;
                    }
                    //cardPlayed = new Card(tempCardPlayed->getCardStr());
                    cardPlayed = new Card(*tempCardPlayed);
                    for (int j = 0; j < gmSt->getNumPlyrs(); j++) {
                        tempScores[j] = 0;
                    }

                } else if (scores[currPosition] == tempScores[currPosition]){
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
                        //cardPlayed = new Card(tempCardPlayed->getCardStr());
                        cardPlayed = new Card(*tempCardPlayed);
                        for (int j = 0; j < gmSt->getNumPlyrs(); j++){
                            tempScores[j] = 0;
                        }
				    } else {
                        for (int j = 0; j < gmSt->getNumPlyrs(); j++){
                            scores[j] = tempScores[j];
                            tempScores[j] = 0;
                        }
				    }
				} else {
                    // restore tempScores into scores
                    for (int j = 0; j < gmSt->getNumPlyrs(); j++){
                        scores[j] = tempScores[j];
                        tempScores[j] = 0;
                    }
                }
				
			} else { // base case - end of game
			    //cardPlayed = new Card(tempCardPlayed->getCardStr());
			    cardPlayed = new Card(*tempCardPlayed);
                gmSt->calcFinalScores();
                if (gmSt->getFinalScore(currPosition) > scores[currPosition]){
                    for (int j = 0; j < gmSt->getNumPlyrs(); j++) {
                        scores[j] = gmSt->getFinalScore(j);
                    }
                }
			}

			gmSt->reversePlay(tempCardPlayed->getCardStr());
		}

		delete tempCardPlayed;
		tempCardPlayed = nullptr;
	}

    return cardPlayed;
}