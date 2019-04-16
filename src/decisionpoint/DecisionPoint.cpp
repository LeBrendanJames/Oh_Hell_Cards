#include "DecisionPoint.h"

DecisionPoint::DecisionPoint(GameState *currGmSt){
	position = currGmSt->getNextToAct();

	for (int i = 0; i < currGmSt->getNumPlyrs(); i++){
		scores.push_back(-1);
	}

	gmSt = new GameState(*currGmSt); // Gamestate copied when decisionPoint constructed

    tie = false;
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

bool DecisionPoint::isTie() {
    return tie;
}

// Loop for at least DEFAULT_BID_SIMULATIONS number of randomly generated games
// where we find hero's best bid, then additional loops in chunks of 'runChunk'
// until oen bid is more common than any other to a statistically sugnificant degree
int DecisionPoint::recommendBid() {
    int optimalBidCount[gmSt->getTotalCards() + 1] {0}; // count of possible bids from 0 to totalCards held in array
    int totalRuns = 0, runChunk = 5;
    int tempBid = -1;
    DecisionPoint * newDPoint = nullptr;

    while (totalRuns < DEFAULT_BID_SIMULATIONS ||
           !statSignificantResult(optimalBidCount, gmSt->getTotalCards() + 1)){
        for (int i = 0; i < runChunk; i++) {
            newDPoint = new DecisionPoint(gmSt);

            tempBid = newDPoint->findBestBid();
            optimalBidCount[tempBid]++;

            delete newDPoint;
        }
        totalRuns += runChunk;
    }

    // return index of the max element in optimalBidCount array (= best bid)
    return std::distance(optimalBidCount, std::max_element(optimalBidCount, optimalBidCount + gmSt->getTotalCards() + 1));
}

// Check if most common bid result happens more often than the second most common
// bid result to a statistically significant degree
bool DecisionPoint::statSignificantResult(int * optimalBidCount, int size){
    // Find largest element and toptwo elements
    int largest = *(std::max_element(optimalBidCount, optimalBidCount + size));
    int topTwo = largest + secondLargestElement(optimalBidCount, size);

    // No formal math behind this,
    // just seems like a reasonable threshold that scales down as you do more runs
    if ((largest / static_cast<double>(topTwo)) >= ((100 - topTwo)/100.0 + 1) / 2.0){
        return true;
    }

    return false;
}

// Find second largest value in array and return its value
int DecisionPoint::secondLargestElement(int * optimalCountArr, int size){
    int largestIdx, secondIdx;

    if (size < 2){
        return 0;
    }

    if (optimalCountArr[0] > optimalCountArr[1]){
        largestIdx = 0;
        secondIdx = 1;
    } else {
        largestIdx = 1;
        secondIdx = 0;
    }

    for (int i = 2; i < size; i++){
        if (optimalCountArr[i] > optimalCountArr[largestIdx]){
            secondIdx = largestIdx;
            largestIdx = i;
        } else if (optimalCountArr[i] > optimalCountArr[secondIdx] && i != largestIdx){
            secondIdx = i;
        }
    }

    return optimalCountArr[secondIdx];
}

int DecisionPoint::findBestBid(){
	int optimalBid = -1;
    int simulationScores[gmSt->getNumPlyrs()] {-1};

	// Generate random hands for all opponents
    if (!gmSt->allHandsGenerated()) {
        bool handsGenerated = genOpponentHands();
        if (!handsGenerated){
            return -1;
        }
    }

    int i = gmSt->getTotalCards(); // max possible bid
    // Loop will cut out early if the amount bid results in the player getting the
    // bonus for winning as many tricks as he bids. Obviously if you get the bonus
    // that's what you want to bid
    while (i >= 0 && scores[this->position] <= BID_CORRECT_BONUS){
        simulateBid(i, simulationScores);

        if (simulationScores[this->position] > scores[this->position]){
            optimalBid = i;
            replaceScores(simulationScores);
        }

        i--;
	}

	return optimalBid;
}

void DecisionPoint::simulateBid(int bid, int* simulatedScores){
    GameState * newGmSt = new GameState(*gmSt);
    DecisionPoint * newDPoint = nullptr;

    if (gmSt->getBid(gmSt->getNextToAct()) == -1) { // if nextToAct has not bid
        newGmSt->makeBid(bid); // make bid for next to act
        newDPoint = new DecisionPoint(newGmSt); // put gameState in decisionpoint
        newDPoint->findBestBid(); // simulate recursively to see what player would score with bid 'i'
    } else {
        newDPoint = new DecisionPoint(newGmSt);
        newDPoint->findBestPlay();
    }

    for (int i = 0; i < gmSt->getNumPlyrs(); i++){
        simulatedScores[i] = newDPoint->getScore(i);
    }
}

void DecisionPoint::replaceScores(int * simulationScores){
    for (int i = 0; i < gmSt->getNumPlyrs(); i++){
        scores[i] = simulationScores[i];
    }
}

// Called within the first call to findBestPlay, so every player has a bid
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
    // Copy over any played cards from gmSt back into masterGmSt plyrHands 2D array
    for (int i = 0; i < gmSt->getTotalCards(); i++){
        for (int j = 0; j < gmSt->getNumPlyrs(); j++){
            if (gmSt->getCardFromPlydCrds(i, j) != nullptr){
                masterGmSt->addCardToPlyrHand(j, gmSt->getCardFromPlydCrds(i, j)->getCardStr());
            }
        }
    }


    // For each player, produce a hand that matches their bid and copy it in to masterGmSt
    for (int i = 0; i < gmSt->getNumPlyrs(); i++) {
        if (masterGmSt->getBid(i) == -1){ // Player hasn't bid yet, so hand is totally random
            addRandomHand(masterGmSt, i);
        } else {
            GameState *indivGmSt = nullptr;
            DecisionPoint *newDPoint = nullptr;
            int noMatchRemaining = 30;
            bool handGenerated = false;
            std::vector<std::string> tempBestHand;

            do {
                // Make a copy of masterGameState, delete bids for player in question + any acting after player
                indivGmSt = new GameState(masterGmSt->getNumPlyrs(), masterGmSt->getHeroPosition(),
                                          masterGmSt->getTotalCards(), masterGmSt->getFlippedCard());
                // For all players up to and including current position in question, copy over known cards in hand
                for (int j = 0; j <= i; j++) {
                    for (int k = 0; k < masterGmSt->getTotalCards(); k++) {
                        if (masterGmSt->getCardFromPlyrHands(j, k) != nullptr) {
                            indivGmSt->addCardToPlyrHand(j, masterGmSt->getCardFromPlyrHands(j, k)->getCardStr());
                        }
                    }
                }

                // put random hands in for everyone from player in question forward
                // Important to put hands in for everyone so that when calling makeBid() we don't loop back into
                // genOpponentHands recursively
                for (int j = i; j < indivGmSt->getNumPlyrs(); j++) {
                    addRandomHand(indivGmSt, j); // Adds hand that doesn't have cards prev used in indivGmSt or gmSt
                }

                // Make sure that I've got gameState set on the correct player next to act before creating DecisionPoint
                for (int j = 0; j < masterGmSt->getNumPlyrs(); j++) {
                    indivGmSt->setBid(j, -1);
                    if (j < i){
                        indivGmSt->makeBid(masterGmSt->getBid(j));
                    }
                }

                // Make a decisionPoint with that and check that the bid from that player would be correct
                newDPoint = new DecisionPoint(indivGmSt);
                int bidDiff = abs(newDPoint->findBestBid() - masterGmSt->getBid(i));

                if (bidDiff == 0){
                    handGenerated = true;
                } else if (bidDiff <= 2){
                    noMatchRemaining--;
                    noMatchRemaining = std::min(noMatchRemaining, bidDiff * 5);
                }
                // Save hand somehow
                tempBestHand.clear();
                for (int j = 0; j < indivGmSt->getTotalCards(); j++){
                    tempBestHand.push_back(indivGmSt->getCardFromPlyrHands(i, j)->getCardStr());
                }
                // Reset indivGmSt and newDPoint
                delete indivGmSt;
                indivGmSt = nullptr;
                delete newDPoint;
                newDPoint = nullptr;
            } while (!handGenerated && noMatchRemaining > 0);

            // Take that generated hand and place it in the first copied gameState (at top of function)
            // Still need to do the above for hero so that positions after can look at hero's hand as random,
            // but actual game will obviously have just normal hero's hand
            for (int j = 0; j < masterGmSt->getTotalCards(); j++){
                masterGmSt->addCardToPlyrHand(i, tempBestHand[j]);
            }
        }
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

Card* DecisionPoint::recommendPlay(){
    int optimalPlayCount[gmSt->getCardsRemaining()] {0};
    int runCount = 0, simCount = 0;
    Card * optimalCard = nullptr;
    DecisionPoint * newDPoint = nullptr;

    // Keep running if
    // 1. runCount < default
    // 2. runCount > 2 and is accruing fast enough
        // runCount * 5 being less than simCount would mean there's a lot of ties
        // so I should just go with whatever is winning every onece in a while
    while (runCount < DEFAULT_PLAY_SIMULATIONS && (runCount <= 2 || (runCount > 0 && runCount * 5 > simCount))){
        runPlaySim(optimalPlayCount, runCount, simCount);
    }

    while (!statSignificantResult(optimalPlayCount, gmSt->getCardsRemaining())){
        runCount = 0;
        while (runCount < 5){
            runPlaySim(optimalPlayCount, runCount, simCount);
        }
    }

    return gmSt->getCardFromPlyrHands(gmSt->getNextToAct(),
                                      std::distance(optimalPlayCount,
                                                    std::max_element(optimalPlayCount,
                                                                     optimalPlayCount + gmSt->getCardsRemaining())));
}

void DecisionPoint::runPlaySim(int * optimalPlayCount, int& runCount, int& simCount){
    DecisionPoint * newDPoint = new DecisionPoint(gmSt);
    simCount++;
    Card * optimalCard = newDPoint->findBestPlay();
    if (!newDPoint->isTie()) { // If there wasn't a tie for best play
        runCount++;
        for (int i = 0; i < gmSt->getCardsRemaining(); i++) {
            if (*optimalCard == *(gmSt->getCardFromPlyrHands(gmSt->getNextToAct(), i))) {
                optimalPlayCount[i]++;
                break;
            }
        }
    }
    delete newDPoint;
    delete optimalCard;
    optimalCard = nullptr;
}

Card* DecisionPoint::findBestPlay() {
    if (!gmSt->allHandsGenerated()){
        bool handsGenerated = genOpponentHands();
        if (!handsGenerated){
            return nullptr;
        }
    }

    Card* cardPlayed = makePlay();

    return cardPlayed;
}


Card* DecisionPoint::makePlay(){
    int currBestCard = -1;
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

	// Loop through all potential cards available
    for (int i = 0; i < gmSt->getCardsRemaining(); i++){
		if (gmSt->isLastTrick()){ // base case
		    currBestCard = i;
		    // play out last round
            while (gmSt->getNextToAct() != -1){
                gmSt->playCard(0);
            }
            gmSt->calcFinalScores();
            if (gmSt->getFinalScore(currPosition) > scores[currPosition]){
                for (int j = 0; j < gmSt->getNumPlyrs(); j++) {
                    scores[j] = gmSt->getFinalScore(j);
                }
            }
            // reverse out last round
            for (int j = 0; j < gmSt->getNumPlyrs(); j++){
                gmSt->reversePlay();
            }
		} else {
            // if this returns true, then play has been made
            // false means the play was invalid for some reason, so we just skip it and move to the next card
            if ((gmSt->playCard(i))) {
                for (int j = 0; j < gmSt->getNumPlyrs(); j++) {
                    tempScores[j] = scores[j];
                }

                makePlay();

                tie = false;

                if (scores[currPosition] > tempScores[currPosition]) {
                    currBestCard = i;
                } else {
                    // If the scores are equal, then playing either card is fine,
                    // so I just leave it as whatever card is currently chosen
                    if (scores[currPosition] == tempScores[currPosition]) {
                        tie = true;
                    }
                    for (int j = 0; j < gmSt->getNumPlyrs(); j++) {
                        scores[j] = tempScores[j];
                    }
                }
                gmSt->reversePlay();
            }
        }
	}

	// As cards are played and plays reversed, the order of cards in a player's hand is reversed
    // So, position of card i is at getCardsRemaining() - original position - 1
	cardPlayed = new Card(*(gmSt->getCardFromPlyrHands(gmSt->getNextToAct(), gmSt->getCardsRemaining() - currBestCard - 1)));
    return cardPlayed;
}