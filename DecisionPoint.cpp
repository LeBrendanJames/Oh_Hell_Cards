

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
    //std::cout << "In DecisionPoint::makeBid()" << std::endl;
    //std::cout << "Current bids:" << std::endl;
    //for (int i = 0; i < gmSt->getNumPlyrs(); i++){
    //    std::cout << "Player #" << i << ": " << gmSt->getBid(i) << std::endl;
    //}

	int optimalBid = -1;

    if (!gmSt->allHandsGenerated()) {
        //std::cout << "Calling genOpponentHands() from makeBid()" << std::endl;
        bool handsGenerated = genOpponentHands();
        if (!handsGenerated){
            return -1;
        }
        //std::cout << "Finished with generating hands" << std::endl;
    }
	
	// for bid = 0 to totalCards
	for (int i  = 0; i <= gmSt->getTotalCards(); i++){
		// copy gameState
		GameState * newGmSt = new GameState(*gmSt);
		
		DecisionPoint * newDPoint = nullptr;
		
		if (gmSt->getBid(gmSt->getNextToAct()) == -1) {
		    //std::cout << "In if stmt in makeBid()" << std::endl;

		    //std::cout << "About to make bid for position " << newGmSt->getNextToAct() << " of " << i << std::endl;

			// copiedGmSt->makeBid(i) // **THIS IS THE EQUIVALEND OF NEWGMST->PLAYCARD() IN MAKEPLAY()
			newGmSt->makeBid(i);

			//std::cout << "Made bid of " << i << std::endl;
			
			// make newDecisionPoint with copied gameState
            // Note: New DecisionPoint must be made after newGmSt has been updated with the bid
            // so that it is simulating from the next player to act
			newDPoint = new DecisionPoint(newGmSt);

			//std::cout << "Made new DecisionPoint" << std::endl;

			// newDPoint->makeBid()
			newDPoint->makeBid();
			
		} else {
		    //std::cout << "In else stmt in DecisionPoint::makeBid() that is going to call makePlay" << std::endl;
			// make newDecisionPoint with copied gameState 
			newDPoint = new DecisionPoint(newGmSt);
			
			// newDecisionPoint->makePlay()
			Card * playReturn = newDPoint->makePlay(); // playCard (the equivalent of makeBid, above) happens within this function & updates newGmSt
			//if (playReturn == nullptr){
			//    return -1;
			//}
		}

		//std::cout << "Am I here when I crash?" << std::endl;
		
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
		
	//std::cout << "Returning " << optimalBid << " from makeBid()" << std::endl;
	return optimalBid;
}

/*
// TODO: More advanced card generation will take into account a player's bidirectional_iterator
// For example, there is only some subset of hands that will optimally bid 0, so make sure
// that the card generation algorithm generates one of that set of hands if the player has bid 0.
bool DecisionPoint::genOpponentHands() {
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
*/

// Called within the first call to makePlay, so every player has a bid
// and potentially some players have already played a card in the first round
bool DecisionPoint::genOpponentHands(){
    //std::cout << "In genOpponentHands" << std::endl;
    //std::cout << "GameState passed in:" << std::endl;
    //std::cout << "Bids:" << std::endl;
    //for (int i = 0; i < gmSt->getNumPlyrs(); i++){
    //    std::cout << "PLayer #" << i << ": " << gmSt->getBid(i) << std::endl;
    //}
    //std::cout << "Hands:" << std::endl;
    //for (int i = 0; i < gmSt->getNumPlyrs(); i++){
    //    std::cout << "Player #" << i << ": ";
    //    for (int j = 0; j < gmSt->getCardsRemaining(); j++){
    //        if (gmSt->getCardFromPlyrHands(i, j) != nullptr) {
    //            std::cout << gmSt->getCardFromPlyrHands(i, j)->getCardStr() << " ";
    //        } else {
    //            std::cout << "-- ";
    //        }
    //    }
    //    std::cout << std::endl;
    //}
    //std::cout << "Played Cards:" << std::endl;
    //for (int i = 0; i < gmSt->getTotalCards(); i++){
    //    std::cout << "Round #" << i + 1 << ": ";
    //    for (int j = 0; j < gmSt->getNumPlyrs(); j++){
    //        if (gmSt->getCardFromPlydCrds(i, j) != nullptr) {
    //            std::cout << gmSt->getCardFromPlydCrds(i, j)->getCardStr() << " ";
    //        } else {
    //            std::cout << "-- ";
    //        }
    //    }
    //    std::cout << std::endl;
    //}
    //std::cout << "Next to act: " << gmSt->getNextToAct() << std::endl;
    //std::cout << std::endl;

    // Make a copy of gameState
        // To hold randomly generated hands that match bids (for all players, including hero)
        // So, this copied gameState holds bids for everyone, but no hands yet
    GameState * masterGmSt = new GameState(gmSt->getNumPlyrs(), gmSt->getHeroPosition(),
                                           gmSt->getTotalCards(), gmSt->getFlippedCard());
    //GameState * masterGmSt = new GameState(*gmSt);
    //masterGmSt->deleteAllHands();

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

    //std::cout << "Through copying gamestate and deleting all hands" << std::endl;
    //std::cout << "Bids:" << std::endl;
    //for (int j = 0; j < masterGmSt->getNumPlyrs(); j++){
    //    std::cout << "Player #" << j << ": " << masterGmSt->getBid(j) << std::endl;
    //}

    // **Maybe need to copy over plydCrds as well for when making recommendation halfway through game?**


    //std::cout << "Made masterGmSt:" << std::endl;
    //std::cout << "Bids:" << std::endl;
    //for (int i = 0; i < masterGmSt->getNumPlyrs(); i++){
    //    std::cout << "PLayer #" << i << ": " << masterGmSt->getBid(i) << std::endl;
    //}
    //std::cout << "Hands:" << std::endl;
    //for (int i = 0; i < masterGmSt->getNumPlyrs(); i++){
    //    std::cout << "Player #" << i << ": ";
    //    for (int j = 0; j < masterGmSt->getCardsRemaining(); j++){
    //        if (masterGmSt->getCardFromPlyrHands(i, j) != nullptr) {
    //            std::cout << masterGmSt->getCardFromPlyrHands(i, j)->getCardStr() << " ";
    //        } else {
    //            std::cout << "-- ";
    //        }
    //    }
    //    std::cout << std::endl;
    //}
    //std::cout << "Played Cards:" << std::endl;
    //for (int i = 0; i < masterGmSt->getTotalCards(); i++){
    //    std::cout << "Round #" << i + 1 << ": ";
    //    for (int j = 0; j < masterGmSt->getNumPlyrs(); j++){
    //        if (masterGmSt->getCardFromPlydCrds(i, j) != nullptr) {
    //            std::cout << masterGmSt->getCardFromPlydCrds(i, j)->getCardStr();
    //        } else {
    //            std::cout << "-- ";
    //        }
    //    }
    //    std::cout << std::endl;
    //}
    //std::cout << "Next to act: " << masterGmSt->getNextToAct() << std::endl;
    //std::cout << std::endl;


    // For each player, produce a hand that matches their bid and copy it in to masterGmSt
    for (int i = 0; i < gmSt->getNumPlyrs(); i++) {
        GameState * indivGmSt = nullptr;
        DecisionPoint * newDPoint = nullptr;

        int noMatchCount = 0;

        // do
        do {
            //std::cout << "Bids don't match. Back at top of do while loop" << std::endl;
            noMatchCount++;

            // Reset indivGmSt and newDPoint
            delete indivGmSt;
            indivGmSt = nullptr;
            delete newDPoint;
            newDPoint = nullptr;

            // Make a copy of masterGameState, delete bids for player in question + any acting after player
            indivGmSt = new GameState(*masterGmSt);

            //std::cout << "Made indivGmSt:" << std::endl;
            //std::cout << "Bids:" << std::endl;
            //for (int i = 0; i < indivGmSt->getNumPlyrs(); i++){
            //    std::cout << "PLayer #" << i << ": " << indivGmSt->getBid(i) << std::endl;
            //}
            //std::cout << "Hands:" << std::endl;
            //for (int i = 0; i < indivGmSt->getNumPlyrs(); i++){
            //    std::cout << "Player #" << i << ": ";
            //    for (int j = 0; j < indivGmSt->getCardsRemaining(); j++){
            //        if (indivGmSt->getCardFromPlyrHands(i, j) != nullptr) {
            //            std::cout << indivGmSt->getCardFromPlyrHands(i, j)->getCardStr() << " ";
            //        } else {
            //            std::cout << "-- ";
            //        }
            //    }
            //    std::cout << std::endl;
            //}
            //std::cout << "Played Cards:" << std::endl;
            //for (int i = 0; i < indivGmSt->getTotalCards(); i++){
            //    std::cout << "Round #" << i + 1 << ": ";
            //    for (int j = 0; j < indivGmSt->getNumPlyrs(); j++){
            //        if (indivGmSt->getCardFromPlydCrds(i, j) != nullptr) {
            //            std::cout << indivGmSt->getCardFromPlydCrds(i, j)->getCardStr();
            //       } else {
            //            std::cout << "-- ";
            //        }
            //    }
            //    std::cout << std::endl;
            //}
            //std::cout << "Next to act: " << indivGmSt->getNextToAct() << std::endl;
            //std::cout << std::endl;


            // put random hands in for everyone from player in question forward
                // Important to put hands in for everyone so that when calling makeBid() we don't loop back into
                // genOpponentHands recursively
                // **make use of 'excludedCards' to make sure I'm not generating hands that contain
                // cards that are in heroHand in gmSt**
            for (int j = i; j < indivGmSt->getNumPlyrs(); j++) {
                addRandomHand(indivGmSt, j); // Adds hand that doesn't have cards previously used in indivGmSt or gmSt
            }


            //std::cout << "Generated hands in indivGmSt:" << std::endl;
            //std::cout << "Bids:" << std::endl;
            //for (int i = 0; i < indivGmSt->getNumPlyrs(); i++){
            //    std::cout << "PLayer #" << i << ": " << indivGmSt->getBid(i) << std::endl;
            //}
            //std::cout << "Hands:" << std::endl;
            //for (int i = 0; i < indivGmSt->getNumPlyrs(); i++){
            //    std::cout << "Player #" << i << ": ";
            //    for (int j = 0; j < indivGmSt->getCardsRemaining(); j++){
            //        if (indivGmSt->getCardFromPlyrHands(i, j) != nullptr) {
            //            std::cout << indivGmSt->getCardFromPlyrHands(i, j)->getCardStr() << " ";
            //        } else {
            //            std::cout << "-- ";
            //        }
            //    }
            //    std::cout << std::endl;
            //}
            //std::cout << "Played Cards:" << std::endl;
            //for (int i = 0; i < indivGmSt->getTotalCards(); i++){
            //    std::cout << "Round #" << i + 1 << ": ";
            //    for (int j = 0; j < indivGmSt->getNumPlyrs(); j++){
            //        if (indivGmSt->getCardFromPlydCrds(i, j) != nullptr) {
            //            std::cout << indivGmSt->getCardFromPlydCrds(i, j)->getCardStr();
            //        } else {
            //            std::cout << "-- ";
            //        }
            //    }
            //    std::cout << std::endl;
            //}
            //std::cout << "Next to act: " << indivGmSt->getNextToAct() << std::endl;
            //std::cout << std::endl;



            // Make sure that I've got gameState set on the correct player next to act before creating DecisionPoint
            for (int j = 0; j < masterGmSt->getNumPlyrs(); j++){
                indivGmSt->setBid(j, -1);
            }
            for (int j = 0; j < i; j++){ //TODO: I think this can be moved in previous loop within an if (j < i) stmt
                indivGmSt->setBid(j, masterGmSt->getBid(j)); // Sets bid in indivGmSt corrrectly and updates nextToAct
            }
            //indivGmSt->setNextToAct(i);

            //std::cout << "Reset bids so that player making bid can only see bids in front:" << std::endl;
            //std::cout << "Bids:" << std::endl;
            //for (int i = 0; i < indivGmSt->getNumPlyrs(); i++){
            //    std::cout << "PLayer #" << i << ": " << indivGmSt->getBid(i) << std::endl;
            //}
            //std::cout << "Hands:" << std::endl;
            //for (int i = 0; i < indivGmSt->getNumPlyrs(); i++){
            //    std::cout << "Player #" << i << ": ";
            //    for (int j = 0; j < indivGmSt->getCardsRemaining(); j++){
            //        if (indivGmSt->getCardFromPlyrHands(i, j) != nullptr) {
            //            std::cout << indivGmSt->getCardFromPlyrHands(i, j)->getCardStr() << " ";
            //        } else {
            //            std::cout << "-- ";
            //        }
            //    }
            //    std::cout << std::endl;
            //}
            //std::cout << "Played Cards:" << std::endl;
            //for (int i = 0; i < indivGmSt->getTotalCards(); i++){
            //    std::cout << "Round #" << i + 1 << ": ";
            //    for (int j = 0; j < indivGmSt->getNumPlyrs(); j++){
            //        if (indivGmSt->getCardFromPlydCrds(i, j) != nullptr) {
            //            std::cout << indivGmSt->getCardFromPlydCrds(i, j)->getCardStr();
            //        } else {
            //            std::cout << "-- ";
            //        }
            //    }
            //    std::cout << std::endl;
            //}
            //std::cout << "Next to act: " << indivGmSt->getNextToAct() << std::endl;
            //std::cout << std::endl;

            //std::cout << "Created indivGmSt in genOpponentHands. Bids:" << std::endl;
            //for (int j = 0; j < indivGmSt->getNumPlyrs(); j++){
            //    std::cout << "Player #" << j << ": " << indivGmSt->getBid(j) << std::endl;
            //}

            //std::cout << "Set bids for players in front of position I'm testing bid for." << std::endl;

            // Make a decisionPoint with that and check that the bid from that player would be correct
            newDPoint = new DecisionPoint(indivGmSt);

            //std::cout << "Made new DecisionPoint. About to do bottom of do while loop." << std::endl;
            //std::cout << "nextToAct = " << indivGmSt->getNextToAct() << std::endl;
            //std::cout << "Bid coming out of that DecisionPoint: " << newDPoint->makeBid() << std::endl;
            //std::cout << "Desired bid (coming from masterGmSt): " << masterGmSt->getBid(i) << std::endl;

        // while (bid from decisionPoint doesn't match actual bid)
        } while (masterGmSt->getBid(i) != -1 && newDPoint->makeBid() != masterGmSt->getBid(i) && noMatchCount < 300);

        //std::cout << "Dropped out of do while loop in genOpponentHands" << std::endl;

        //if (noMatchCount >= 300){
            //std::cout << "Returning false from genOpponentHands because bid not possible" << std::endl;
            //return false;
        //}

        //std::cout << "Out of do while loop, meaning I have a hand that matches bids" << std::endl;

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

    //std::cout << "Dropped out of loop to generate hands for each player." <<  std::endl;

    //std::cout << "masterGmSt Current Status:" << std::endl;
    //std::cout << "Bids:" << std::endl;
    //for (int i = 0; i < masterGmSt->getNumPlyrs(); i++){
    //    std::cout << "PLayer #" << i << ": " << masterGmSt->getBid(i) << std::endl;
    //}
    //std::cout << "Hands:" << std::endl;
    //for (int i = 0; i < masterGmSt->getNumPlyrs(); i++){
    //    std::cout << "Player #" << i << ": ";
    //    for (int j = 0; j < masterGmSt->getCardsRemaining(); j++){
    //        if (masterGmSt->getCardFromPlyrHands(i, j) != nullptr) {
    //            std::cout << masterGmSt->getCardFromPlyrHands(i, j)->getCardStr() << " ";
    //        } else {
    //            std::cout << "-- ";
    //        }
    //    }
    //    std::cout << std::endl;
    //}
    //std::cout << "Played Cards:" << std::endl;
    //for (int i = 0; i < masterGmSt->getTotalCards(); i++){
    //    std::cout << "Round #" << i + 1 << ": ";
    //    for (int j = 0; j < masterGmSt->getNumPlyrs(); j++){
    //        if (masterGmSt->getCardFromPlydCrds(i, j) != nullptr) {
    //            std::cout << masterGmSt->getCardFromPlydCrds(i, j)->getCardStr();
    //        } else {
    //            std::cout << "-- ";
    //        }
    //    }
    //    std::cout << std::endl;
    //}
    //std::cout << "Next to act: " << masterGmSt->getNextToAct() << std::endl;
    //std::cout << std::endl;


    //std::cout << "Now copying over to gmSt." << std::endl;

    // Take that first copy of gameState and copy everyone except hero's hands over to the actual gameState contained in
    // the calling DecisionPoint
    for (int i = 0; i < gmSt->getNumPlyrs(); i++){
        if (i != gmSt->getHeroPosition()){
            for (int j = 0; j < gmSt->getCardsRemaining(); j++){
                // TODO: update this copying to only copy over new cards, not ones already played
                // This copy is relying on the 'addCardToPlyrHand' function rejecting any cards that have been used
                // It's working, but it's not ideal to rely on that function's error checking
                gmSt->addCardToPlyrHand(i, masterGmSt->getCardFromPlyrHands(i, j)->getCardStr());
            }
        }
    }

    delete masterGmSt;
    masterGmSt = nullptr;

    // returning
    // Only change after this function has been run is that gmSt member variable has all player hands filled
    // with cards that would have bid numbers that match what they actually bid in the game

    //std::cout << "About to return. Status of gmSt: " << std::endl;
    //std::cout << "Bids:" << std::endl;
    //for (int i = 0; i < gmSt->getNumPlyrs(); i++){
    //    std::cout << "PLayer #" << i << ": " << gmSt->getBid(i) << std::endl;
    //}
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
    //std::cout << "Played Cards:" << std::endl;
    //for (int i = 0; i < gmSt->getTotalCards(); i++){
    //    std::cout << "Round #" << i + 1 << ": ";
    //    for (int j = 0; j < gmSt->getNumPlyrs(); j++){
    //    if (gmSt->getCardFromPlydCrds(i, j) != nullptr) {
    //            std::cout << gmSt->getCardFromPlydCrds(i, j)->getCardStr() << " ";
    //        } else {
    //            std::cout << "-- ";
    //        }
    //    }
    //    std::cout << std::endl;
    //}
	//std::cout << "nextToAct in gmSt: " << gmSt->getNextToAct() << std::endl;
    //std::cout << std::endl;

    return true;
}


void DecisionPoint::addRandomHand(GameState * indivGmSt, int position){
    //std::cout << "In DecisionPoint::addRandomHand" << std::endl;

    bool validSuits[4] {true};
    bool validSuit = false;
    int cardVal = -1, cardSuit = -1;
    bool cardPrevUsed = true;

    for (int j = 0; j < 4; j++){ // 4 = 4 suits, so fine to have in there as a constant
        validSuits[j] = true;
    }
    markInvalidSuits(position, validSuits);

    // If player has already played a card this round, their hand will hold one fewer card, so start below loop counter
    // at 1 rather than 0
    int k = 0;
    if (indivGmSt->getCardFromPlydCrds(indivGmSt->getCurrRound(), position) != nullptr){
        k = 1;
    }

    //std::cout << "About to actually generate cards" << std::endl;

    for (int j = k; j < gmSt->getCardsRemaining(); j++){
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
    //std::cout << "In DecisionPoint::makePlay()" << std::endl;
    //std::cout << "Bids are: " << std::endl;
    //for (int i = 0; i < gmSt->getNumPlyrs(); i++){
    //    std::cout << "Player #" << i << ": " << gmSt->getBid(i) << std::endl;
    //}
    //std::cout << "Hands are: " << std::endl;
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
    //std::cout << "nextToAct = " << gmSt->getNextToAct() << std::endl;
	GameState * newGmSt = nullptr;

	if (!gmSt->allHandsGenerated()){
	    //std::cout << "All hands not generated, so calling genOpponentHands" << std::endl;
	    bool handsGenerated = genOpponentHands();

	    if (!handsGenerated){
	        return nullptr;
	    }


	    //std::cout << "Bids are: " << std::endl;
	    //for (int i = 0; i < gmSt->getNumPlyrs(); i++){
	    //    std::cout << "Player #" << i << ": " << gmSt->getBid(i) << std::endl;
	    //}
	    //std::cout << "Hands are: " << std::endl;
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

	}

	//if (gmSt->getCurrRound() == 0){
	//    gmSt->setNextToAct(0);
	//}

	// Loop through all potential cards available
    int numLoops = gmSt->getCardsRemaining();
	//std::cout << "numLoops = " << numLoops << std::endl;
	for (int i = 0; i < numLoops; i++){
        //std::cout << "Within loop in makePlay()" << std::endl;
        //std::cout << "Current state of game:" << std::endl;
        //std::cout << "Bids: " << std::endl;
        //for (int j = 0; j < gmSt->getNumPlyrs(); j++){
        //    std::cout << "Player #" << j << ": " << gmSt->getBid(j) << std::endl;
        //}
        //std::cout << "Hands:" << std::endl;
        //for (int j = 0; j < gmSt->getNumPlyrs(); j++){
        //    std::cout << "Player #" << j << ": ";
        //    for (int k = 0; k < gmSt->getCardsRemaining(); k++){
        //        if (gmSt->getCardFromPlyrHands(j, k) != nullptr) {
        //            std::cout << gmSt->getCardFromPlyrHands(j, k)->getCardStr() << " ";
        //        } else {
        //            std::cout << "-- ";
        //        }
        //    }
        //    std::cout << std::endl;
        //}

		// copy game state
		newGmSt = new GameState(*gmSt);

		Card * tempCardPlayed = new Card(gmSt->getCardFromPlyrHands(gmSt->getNextToAct(), i)->getCardStr());

		// add card played
		bool validPlay = newGmSt->playCard(i);
		//std::cout << "Finished playing card" << std::endl;
		//std::cout << "validPlay = " << validPlay << std::endl;
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
			    //std::cout << "In final else stmt" << std::endl;
			    cardPlayed = new Card(tempCardPlayed->getCardStr());
			    //std::cout << "copied into cardPlayed" << std::endl;
			    //std::cout << "cardPlayed = " << cardPlayed->getCardStr() << std::endl;

                newGmSt->calcFinalScores();

                //std::cout << "Done with calcFinalScores" << std::endl;

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