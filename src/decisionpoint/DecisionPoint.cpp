#include "DecisionPoint.h"

DecisionPoint::DecisionPoint(GameState *currGmSt){
	position = currGmSt->getNextToAct();
	for (int i = 0; i < currGmSt->getNumPlyrs(); i++){
		scores.push_back(-1.0);
	}
	gmSt = new GameState(*currGmSt); // Gamestate copied when decisionPoint constructed
    tie = false;
}

DecisionPoint::~DecisionPoint(){
	delete gmSt;
}


/**********************************************************************************************************************
 * PUBLIC FUNCTIONS
 *********************************************************************************************************************/

/*
 * GETSCORE
 * Getter for score vector (which holds player scores after simulating a full Oh Hell round)
 */
int DecisionPoint::getScore(int index){
    if (index >= 0 && index < scores.size()) {
        return scores[index];
    } else {
        return -1;
    }
}

/*
 * ISTIE
 * Getter to see if the DecisionPoint run resulted in a tie between two different possible card plays
 * Tie variable set within makePlay function if two playes result in same score at end of round
 */
bool DecisionPoint::isTie() {
    return tie;
}

/*
 * RECOMMENDBID
 * Loop for at least DEFAULT_BID_SIMULATIONS number of randomly generated games
 * where we find hero's best bid, then additional loops in chunks of 'runChunk'
 * until one bid is more common than any other to a statistically sugnificant degree
 */
int DecisionPoint::recommendBid() {
    int optimalBidCount[gmSt->getTotalCards() + 1]; // count of possible bids from 0 to totalCards held in array
    for (int i = 0; i <= gmSt->getTotalCards(); i++){
        optimalBidCount[i] = 0;
    }
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

/*
 * RECOMMENDPLAY
 * Loop for at least DEFAULT_PLAY_SIMULATIONS number of randomly generated games
 * where we find hero's best play, then additional loops of 'runchunk' size
 * until one play is more common than any other to a statistically sugnificant degree
 */
Card* DecisionPoint::recommendPlay(){
    int optimalPlayCount[gmSt->getCardsRemaining()] {0};
    int runCount = 0, runChunk = 5;

    while (runCount < DEFAULT_PLAY_SIMULATIONS ||
           !statSignificantResult(optimalPlayCount, gmSt->getCardsRemaining())){
        for (int i = 0; i < runChunk; i++){
            simulatePlay(optimalPlayCount);
        }
        runCount += runChunk;
    }

    // Return the card that has come up the most in the above sims
    return gmSt->getCardFromPlyrHands(gmSt->getNextToAct(),
                                      std::distance(optimalPlayCount,
                                                    std::max_element(optimalPlayCount,
                                                                     optimalPlayCount + gmSt->getCardsRemaining())));
}


/***********************************************************************************************************************
 * PRIVATE FUNCTIONS
 *
 *
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * HELPER FUNCTIONS FOR LOOPING THROUGH GAME SIMULATIONS
 **********************************************************************************************************************/

/*
 * STATSIGNIFICANTRESULT
 * Check if most common bid or play result happens more often than the second most common
 * bid or play result to a statistically significant degree
 */
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

/*
 * SECONDLARGESTELEMENT
 * Find second largest value in array and return its value
 */
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


/**********************************************************************************************************************
 * FINDING BEST BID TO MAKE
 **********************************************************************************************************************/

/*
 * FINDBESTBID
 * Generates random hands for each player, if that hasn't already been done.
 * Then, simulates a full round of OhHell for each possible bid that hero could make,
 * recording the score hero would get if each player played optimally the whole way
 * through the game.
 * Returns the bid that result in the maximum possible score.
 *
 * Implementation note: The function starts with the highest possible bid and
 * returns early if that bid is one that the player will hit perfectly
 * i.e. if a player can bid 2 and win two tricks with everyone playing optimally,
 * then there is no reason to even test bids of 1 or 0 - the player will optimally bid 2.
 */
int DecisionPoint::findBestBid(){
	int optimalBid = -1;
    int simulationScores[gmSt->getNumPlyrs()];
    for (int i = 0; i < gmSt->getNumPlyrs(); i++){
        simulationScores[i] = -1;
    }

	// Generate random hands for all opponents
    if (!gmSt->allHandsGenerated()) {
        genOpponentHands();
    }

    int i = gmSt->getTotalCards(); // max possible bid
    // Loop will cut out early if the amount bid results in the player getting more the
    // points than the max if he were to bid lower. Obviously if you can get more than the
    // max of lower bids, you're not going to bid any lower.
    while (i >= 0 && scores[this->position] <= BID_CORRECT_BONUS + i - 1){
        simulateBid(i, simulationScores);

        if (simulationScores[this->position] > scores[this->position]){
            optimalBid = i;
            replaceScores(simulationScores);
        }

        i--;

        // Don't need to loop through all possible bids for a position where user entered a bid
        if (gmSt->getBid(gmSt->getNextToAct()) == -1){
            break;
        }
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


/**********************************************************************************************************************
 * FINDING BEST CARD TO PLAY
 **********************************************************************************************************************/

void DecisionPoint::simulatePlay(int *optimalPlayCount){
    DecisionPoint * newDPoint = new DecisionPoint(gmSt);
    Card * optimalCard = newDPoint->findBestPlay();
    if (!newDPoint->isTie()) { // If there wasn't a tie for best play
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
    }
    return makePlay();
}



Card* DecisionPoint::makePlay(){
    int currBestCard = -1;
    int currPosition = gmSt->getNextToAct();
    std::vector<int> tempScores;
    tempScores.reserve(gmSt->getNumPlyrs());
    for (int i = 0; i < gmSt->getNumPlyrs(); i++){
        tempScores.push_back(0);
    }

    for (int i = 0; i < gmSt->getNumPlyrs(); i++) {
        scores[i] = -1;
    }

    int tieCount = 0;

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
            std::string tempCardStr = gmSt->getCardFromPlyrHands(gmSt->getNextToAct(), i)->getCardStr();
            int plyrToAct = gmSt->getNextToAct();
            if ((gmSt->playCard(i))) {
                for (int j = 0; j < gmSt->getNumPlyrs(); j++) {
                    tempScores[j] = scores[j];
                }

                Card * tempCardPlayed = makePlay();
                delete tempCardPlayed; // Memory allocted in makePlay needs to be deleted

                tie = false;

                if (scores[currPosition] > tempScores[currPosition]) {
                    currBestCard = i;
                } else if (scores[currPosition] < tempScores[currPosition]){
                    for (int j = 0; j < gmSt->getNumPlyrs(); j++) {
                        scores[j] = tempScores[j];
                    }
                } else { // Tie, so current player is equally likely to play each card
                    // If the scores are equal, then playing either card is fine,
                    // so I just leave it as whatever card is currently chosen
                    tie = true;
                    tieCount++;

                    // avg scores somehow (wtd average though, since tempScores can already be an average
                    for (int j = 0; j < gmSt->getNumPlyrs(); j++) {
                        scores[j] = (scores[j] + tieCount * tempScores[j]) / static_cast<double>(tieCount + 1);
                    }
                }
                gmSt->reversePlay();
            }
        }
	}

	// As cards are played and plays reversed, the order of cards in a player's hand is reversed
    // So, position of card i is at getCardsRemaining() - original position - 1
    return new Card(*(gmSt->getCardFromPlyrHands(gmSt->getNextToAct(), gmSt->getCardsRemaining() - currBestCard - 1)));
}



/**********************************************************************************************************************
 * GENERATING OPPONENT HANDS
 **********************************************************************************************************************/

// Called within the first call to findBestPlay, so every player has a bid
// and potentially some players have already played a card in the first round
bool DecisionPoint::genOpponentHands(){
    // Make gameState that is a reconstruction of the start of the current gmSt
    GameState * masterGmSt = reconstructGmStFromStart();

    // For each player, produce a hand that matches their bid and copy it in to masterGmSt
    for (int i = 0; i < gmSt->getNumPlyrs(); i++) {
        fillPlayerHand(masterGmSt, i);
    }

    // Copy those generated hands back into the original gmSt
    copyOpponentHandsToGmSt(masterGmSt);

    delete masterGmSt;
    masterGmSt = nullptr;

    // Only change after this function has been run is that DecisionPoint's gmSt member object has all player
    // hands filled with cards that would have bid numbers that match what they actually bid in the game
    return true;
}

void DecisionPoint::fillPlayerHand(GameState* masterGmSt, int plyrPosition){
    if (masterGmSt->getBid(plyrPosition) == -1){ // Player hasn't bid yet, so hand is totally random
        addRandomHand(masterGmSt, masterGmSt, plyrPosition);
    } else {
        addHandToMatchBid(masterGmSt, plyrPosition);
    }
}

// Gen hand that matches bid already present in masterGmSt
// i.e. make a hand where if a player were holding it, they would optimally bid whatever they have actually bid
void DecisionPoint::addHandToMatchBid(GameState * masterGmSt, int plyrPosition){
    GameState *indivGmSt = nullptr;
    DecisionPoint *newDPoint = nullptr;
    int noMatchRemaining = 30;
    bool handGenerated = false;
    std::vector<std::string> tempBestHand;

    do {
        indivGmSt = setNewRandomHandGmSt(plyrPosition, masterGmSt);

        // Make a decisionPoint with that and check that the bid from that player would be correct
        newDPoint = new DecisionPoint(indivGmSt);
        int bidDiff = abs(newDPoint->findBestBid() - masterGmSt->getBid(plyrPosition));

        if (bidDiff == 0){
            handGenerated = true;
        } else if (bidDiff <= 2){
            noMatchRemaining--;
            noMatchRemaining = std::min(noMatchRemaining, bidDiff * 5);
        }
        // Save hand
        tempBestHand.clear();
        for (int j = 0; j < indivGmSt->getTotalCards(); j++){
            tempBestHand.push_back(indivGmSt->getCardFromPlyrHands(plyrPosition, j)->getCardStr());
        }
        // Reset indivGmSt and newDPoint
        delete indivGmSt;
        indivGmSt = nullptr;
        delete newDPoint;
        newDPoint = nullptr;
    } while (!handGenerated && noMatchRemaining > 0);

    // Take that generated hand and place it in masterGmSt
    for (int j = 0; j < masterGmSt->getTotalCards(); j++){
        masterGmSt->addCardToPlyrHand(plyrPosition, tempBestHand[j]);
    }
}

GameState* DecisionPoint::setNewRandomHandGmSt(int currPosition, GameState * masterGmSt){
    // Make a copy of masterGameState, delete bids for player in question + any acting after player
    GameState * indivGmSt = new GameState(masterGmSt->getNumPlyrs(), masterGmSt->getHeroPosition(),
                                          masterGmSt->getTotalCards(), masterGmSt->getFlippedCard());
    // For all players up to and including current position in question, copy over known cards in hand
    for (int j = 0; j <= currPosition; j++) {
        for (int k = 0; k < masterGmSt->getTotalCards(); k++) {
            if (masterGmSt->getCardFromPlyrHands(j, k) != nullptr) {
                indivGmSt->addCardToPlyrHand(j, masterGmSt->getCardFromPlyrHands(j, k)->getCardStr());
            }
        }
    }

    // put random hands in for everyone from player in question forward
    // Important to put hands in for everyone so that when calling makeBid() we don't loop back into
    // genOpponentHands recursively
    for (int j = currPosition; j < indivGmSt->getNumPlyrs(); j++) {
        addRandomHand(indivGmSt, masterGmSt, j); // Adds hand that doesn't have cards prev used in indivGmSt or gmSt
    }

    // Make sure that I've got gameState set on the correct player next to act before creating DecisionPoint
    for (int j = 0; j < currPosition; j++) {
        indivGmSt->makeBid(masterGmSt->getBid(j));
    }

    return indivGmSt;
}

GameState* DecisionPoint::reconstructGmStFromStart(){
    // Make a copy of gameState
    // To hold randomly generated hands that match bids (for all players, including hero)
    // So, this copied gameState holds bids for everyone, but no hands yet
    GameState * startGmSt = new GameState(gmSt->getNumPlyrs(), gmSt->getHeroPosition(),
                                          gmSt->getTotalCards(), gmSt->getFlippedCard());
    // copy over bids
    for (int i = 0; i < startGmSt->getNumPlyrs(); i++){
        startGmSt->setBid(i, gmSt->getBid(i));
    }
    // Copy over any played cards from gmSt back into masterGmSt plyrHands 2D array
    for (int i = 0; i < gmSt->getTotalCards(); i++){
        for (int j = 0; j < gmSt->getNumPlyrs(); j++){
            if (gmSt->getCardFromPlydCrds(i, j) != nullptr){
                startGmSt->addCardToPlyrHand(j, gmSt->getCardFromPlydCrds(i, j)->getCardStr());
            }
        }
    }

    return startGmSt;
}

void DecisionPoint::copyOpponentHandsToGmSt(GameState * masterGmSt){
    for (int i = 0; i < gmSt->getNumPlyrs(); i++){
        if (i != gmSt->getHeroPosition()){
            for (int j = 0; j < gmSt->getTotalCards(); j++){
                gmSt->addCardToPlyrHand(i, masterGmSt->getCardFromPlyrHands(i, j)->getCardStr());
            }
        }
    }
}


void DecisionPoint::addRandomHand(GameState * indivGmSt, GameState * masterGmSt, int position){
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
            // Generate random card, making sure it hasn't already been used & its suit is valid
            // (based on what player has previously played)
            do {
                cardVal = rand() % 13 + 1;
                cardSuit = rand() % 4 + 1;
            } while (cardAlreadyUsed(gmSt, indivGmSt, masterGmSt, cardVal, cardSuit) ||
                     !isValidSuit(cardSuit, validSuits));

            // Add to plyrHands array
            Card *cardToAdd = new Card(cardVal, cardSuit);
            indivGmSt->addCardToPlyrHand(position, cardToAdd->getCardStr());

            delete cardToAdd;
            cardToAdd = nullptr;
        }
    }
}

bool DecisionPoint::cardAlreadyUsed(
        GameState * gmSt, GameState * indivGmSt, GameState * masterGmSt, int cardVal, int cardSuit){
    if (gmSt->cardPrevUsed(cardVal, cardSuit)){
        return true;
    }
    if (indivGmSt->cardPrevUsed(cardVal, cardSuit)){
        return true;
    }
    if (masterGmSt->cardPrevUsed(cardVal, cardSuit)){
        return true;
    }
    return false;
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
            if (gmSt->getCardFromPlydCrds(round, position)->getSuit() !=
                gmSt->getCardFromPlydCrds(round, gmSt->getRoundLead(round))->getSuit()){
                // mark led suit as invalid, since player can't have any of that suit (since they didn't follow suit)
                validSuits[gmSt->getCardFromPlydCrds(round, gmSt->getRoundLead(round))->getSuit()] = false;
            }
        }
        round++;
    }
}

bool DecisionPoint::isValidSuit(int cardSuit, bool * validSuits){
    if (validSuits[cardSuit - 1]){
        return true;
    }
    return false;
}