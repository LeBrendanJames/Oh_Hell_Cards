

#include "Player.h"


Player::Player(bool hero, int position) {
    this->hero = hero;
    this->position = position;
}

bool Player::isHero() {
    return hero;
}

int Player::getPosition() {
    return position;
}

void Player::setHand(GameState * currState, std::string * inputCards){
    if(isHero() && inputCards != nullptr){
        for (int i = 0; i < currState->getTotalCards(); i++) {
            plyrHand.push_back(Card(inputCards[i]));
        }
    } else {
        // Random cards
        srand(time(NULL));
        int suitNum = rand() % 4;
        Suit newSuit;
        if (suitNum == 0) {
            newSuit = Clubs;
        } else if (suitNum == 1){
            newSuit = Diamonds;
        } else if (suitNum == 2){
            newSuit = Hearts;
        } else { // 3
            newSuit = Spades;
        }
        plyrHand.push_back(Card(rand() % 13 + 1, newSuit));
        for (int i = 1; i < currState->getTotalCards(); i++){ // For each card
            int newVal = 0;
            bool matchPrevCard = false;
            do {
                newVal = rand() % 13 + 1; // gen value
                suitNum = rand() % 4; // gen suit
                for (int j = i - 1; j >= 0; j++) { // check it isn't duplicate of previous cards
                    if (newVal == plyrHand[j].getVal() && newSuit == plyrHand[j].getSuit()){
                        matchPrevCard = true;
                        break;
                    }
                }
            } while (matchPrevCard);
            if (suitNum == 0) {
                newSuit = Clubs;
            } else if (suitNum == 1){
                newSuit = Diamonds;
            } else if (suitNum == 2){
                newSuit = Hearts;
            } else { // 3
                newSuit = Spades;
            }
            plyrHand.push_back(Card(rand() % 13 + 1, newSuit));
        }
    }
}

void Player::printHand(){
    for (int i = 0; i < plyrHand.size(); i++){
        std::cout << plyrHand[i].getCardStr() << std::endl;
    }
}

// Make optimal bid based on findings from game simulations
int Player::makeBid(GameState * currState) {
    int bidChoice = -1;
	
	auto ** masterExPts = new double[currState->getTotalCards()];
	for (int i = 0; i < currState->getTotalCards(); i++){
		masterExPts[i] = new double[currState->getNumPlyrs()] {0.0}; // I think this sets all to 0.0? Check in tests...
	}

    // for number of sims
	for (int i = 0; i < NUM_SIMS; i++){
        // Copy gameState (to have one of the players own to use)
		auto * newGmSt = new GameState(*currState);
		
        // Randomize other players cards (and fill hero w/ known hand)
		Card ** allHands;
		makeRandomPlayerHands(&allHands);
		
        // Run simulation (can make this its own function - return array of expected points by bid amount)
		runSimulation(newGmSt, &allHands, &masterExPts);
	}

	// Find which bid produces highest expected points 
	bidChoice = 0;
	for (int i = 1; i < currState->getTotalCards(); i++){
		if (masterExPts[i][position] > masterExPts[bidChoice][position]){
			bidChoice = i;
		}
	}


    return bidChoice;
}

void makeRandomPlayerHands(Card *** otherHands){
	
}

void Player::runSimulation(){
	// Copy gameState
	auto * newGmSt = new GameState(*currState);
	
	// if bid = blank then
	if (bids[newGmSt->getNextToAct()] == -1){
		// for bid = 0 up to max
		for (int i = 0; i < newGmSt->getTotalCards(); i++){
			// update gameState for bid
			bids[newGmSt->getNextToAct()] = i;
			newGmSt->incNextToAct();
			
			// exPts[bidNum] = runSimulation(new gameState)
			runSimulation(newGmSt, &allHands, &masterExPts); // RECURSIVE CALL
		}
	} else { // else
		findPlay(newGmSt, allHands, masterExPts);
	}
	
	// fill passed in results array
	
}

void Player::findPlay(){
	// for play = first available card to last available card
	for (int i = 0; i < newGmSt->getCardsRemaining(); i++){
		// copy gameState
		
		// Update gameState with card played
		newGmSt->addCardPlayed(card, newGmSt->getNextToAct());
		allHands[i][newGmSt->getNextToAct()] = nullptr; // card now pointed to by prevCardsPlayed
		
		// Update who is next to act
		newGmSt->updateNextToAct(); // This either just increments player or if we're at the end of the hand/game figures out next to go 
									// This also decrements cards remaining if we are at the end of a trick 
		
		if (newGmSt->getNextToAct() != -1){
			// results += find play (this is the function where the recursion eventually bottoms out)
			findPlay(newGmSt, allHands, masterExPts);
		} else { // end of game - bottom out of recursion 
			newGmSt->tallyPoints(); // Tallies points or sets all to 0 if cards played order created ineligible game (for example, not following suit when a player could do so) 
		}
		
		// figure out if scenario that bottomed out made sense for
		if (newGmSt->getPts(newGmSt->getNextToAct()) > currPts){
			// update 
		}
	}
	// return row of results that's best for whichever player is the one to act here
}


/*
double Player::simDecision(Card * cardPlayed) {
    // Make theoretical gameState (current gameState + card played)
    // create game with all the same attributes except playerVantagePoint changes
    if (!endRound){
        for (int i = 0; i < currState->getCardsRemaining(); i++){
            simDecision();
        }
        // return max of simDecision() above (which i is max = which card to play)
    } else if (endRound && currState->getCardsRemaining() > 1) {
        // play last card

        // figure out who wins trick and update game state
        leadPosition = currState->determineTrickWinner();

        // simGame from that point, passing in who's play it is
        for (int i = 0; i < currState->getCardsRemaining(); i++){
            simDecision();
        }
        // return max of simDecision()
    }
    // base case is last player plays and points are tallied
    // Pass hero points back up
    else {
        // play last card
        currState->playCard(card, position);

        // tally up points

        return heroPts;
    }
    // Copy game state

    // Copy player into new instance

    // Recursively call makePlay for next player with all possible hands they could have

}
*/

