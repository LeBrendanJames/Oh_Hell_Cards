#include "DecisionPoint.h"

DecisionPoint::DecisionPoint(GameState *currGmSt){
	position = currGmSt->getNextToAct();
	for (int i = 0; i < currGmSt->getNumPlyrs(); i++){
		scores.push_back(-1.0);
	}
	gmSt = new GameState(*currGmSt); // Gamestate copied when decisionPoint constructed
    tie = false;

    bonusProb = new double*[currGmSt->getNumPlyrs()];
    for (int i = 0; i < currGmSt->getNumPlyrs(); i++){
        bonusProb[i] = new double[currGmSt->getTotalCards() + 1];
        for (int j = 0; j < currGmSt->getTotalCards() + 1; j++){
            bonusProb[i][j] = 0.0;
        }
    }

    //std::cout << "numPlyrs: " << currGmSt->getNumPlyrs() << ", totalCards: " << currGmSt->getTotalCards() << std::endl;
    //std::cout << "Allocating " << currGmSt->getNumPlyrs() * (currGmSt->getTotalCards() - 1);
    //std::cout << " rows for tempBonusProbs" << std::endl;
    tempBonusProbs = new double*[currGmSt->getNumPlyrs() * currGmSt->getNumPlyrs() * (currGmSt->getTotalCards() - 1)];
    for (int i = 0; i < currGmSt->getNumPlyrs() * currGmSt->getNumPlyrs() * (currGmSt->getTotalCards() - 1); i++){
        tempBonusProbs[i] = new double[currGmSt->getTotalCards() + 1];
        for (int j = 0; j < currGmSt->getTotalCards() + 1; j++){
            tempBonusProbs[i][j] = 0.0;
        }
    }
}

DecisionPoint::~DecisionPoint(){
    for (int i = 0; i < gmSt->getNumPlyrs(); i++){
        delete [] bonusProb[i];
    }
    delete [] bonusProb;
    for (int i = 0; i < gmSt->getNumPlyrs() * gmSt->getNumPlyrs() * (gmSt->getTotalCards() - 1); i++){
        delete [] tempBonusProbs[i];
    }
    delete [] tempBonusProbs;
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
 * Tie variable set within simulatePlay function if two playes result in same score at end of round
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
            findBestPlay(optimalPlayCount);
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

	// Generate random hands for all opponents
    if (!gmSt->allHandsGenerated()) {
        genOpponentHands();
    }

    while (gmSt->getBid(gmSt->getNextToAct()) == -1){
        gmSt->makeBid(0);
    }

    simulatePlay(true);

    //optimalBid = 0;
    //for (int i = 1; i < gmSt->getTotalCards() + 1; i++){
    //    if (bonusProb[position][i] > bonusProb[position][optimalBid]){
    //        optimalBid = i;
    //    }
    //}

	//return optimalBid;

    // return index of max element in bonusProb array
    return std::distance(bonusProb[position],
                         std::max_element(&bonusProb[position][0], &bonusProb[position][gmSt->getTotalCards() + 1]));
}


/**********************************************************************************************************************
 * FINDING BEST CARD TO PLAY
 **********************************************************************************************************************/

/*
 * FINDBESTPLAY
 * Call to simulatePlay to recursively find the best card to play.
 * Match that card up to its location in hero's hand and increment
 * that index in optimalBidCount[] array.
 */
void DecisionPoint::findBestPlay(int *optimalPlayCount){
    DecisionPoint * newDPoint = new DecisionPoint(gmSt);
    //tie = false;
    Card * optimalCard = newDPoint->simulatePlay(false);
    //Card * optimalCard = simulatePlay(false);
    if (!newDPoint->isTie()) { // If there wasn't a tie for best play
    //if (!tie){
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

/*
 * SIMULATEPLAY
 * Loop through each of the remaining cards in a player's hand, playing each
 * and recursively calling simulatePlay for the next player to act.
 * Base case is when you get to the last round of play, since the order
 * of cards played is determined at that point.
 * At each base case, calculate the scores for each player, which are
 * compared as you step back up the recursion, choosing the option that produces
 * the max score for the player to act at each step of the recurion (or averaging the scores
 * from multiple options if the player to act is indifferent between plays)
 */
Card* DecisionPoint::simulatePlay(bool bidFlag) {
    if (!gmSt->allHandsGenerated()){
        bool handsGenerated = genOpponentHands();
    }

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

    int tieCount = 0, bidTieCount = 0;

    //double ** tempBonusProb = nullptr;
    //if (bidFlag && !gmSt->isLastTrick()){
    //    // allocate memory
    //    tempBonusProb = new double*[gmSt->getNumPlyrs()];
    //    for (int i = 0; i < gmSt->getNumPlyrs(); i++){
    //        tempBonusProb[i] = new double[gmSt->getTotalCards() + 1];
    //        for (int j = 0; j < gmSt->getTotalCards() + 1; j++){
    //            tempBonusProb[i][j] = 0.0;
    //        }
    //    }
    //}

	// Loop through all potential cards available
    for (int i = 0; i < gmSt->getCardsRemaining(); i++){
		if (gmSt->isLastTrick()){ // BASE CASE
		    currBestCard = i;
		    // play out last round
            while (gmSt->getNextToAct() != -1){
                //std::cout << "Playing card: " << gmSt->getCardFromPlyrHands(gmSt->getNextToAct(), 0)->getCardStr() << std::endl;
                gmSt->playCard(0);
            }
            gmSt->calcFinalScores();
            if (gmSt->getFinalScore(currPosition) > scores[currPosition]){
                for (int j = 0; j < gmSt->getNumPlyrs(); j++) {
                    scores[j] = gmSt->getFinalScore(j);
                }
            }

            if (bidFlag){
                for (int j = 0; j < gmSt->getNumPlyrs(); j++){
                    for (int k = 0; k < gmSt->getTotalCards() + 1; k++){
                        bonusProb[j][k] = 0;
                    }
                    // 100% chance of getting whatever # of tricks the player ends up with (base case)
                    bonusProb[j][gmSt->getFinalScore(j) % BID_CORRECT_BONUS] = 1.0;
                }
            }

            // reverse out last round
            for (int j = 0; j < gmSt->getNumPlyrs(); j++){
                gmSt->reversePlay();
            }
		} else { // RECURSIVE CASE
            std::string tempCardStr = gmSt->getCardFromPlyrHands(gmSt->getNextToAct(), i)->getCardStr();
            int plyrToAct = gmSt->getNextToAct();
            // if the below playCard(i) call returns true, then play has been made
            // false means the play was invalid for some reason, so we just skip it and move to the next card
            if ((gmSt->playCard(i))) {
                //std::cout << "Playing card: " << tempCardStr << std::endl;
                for (int j = 0; j < gmSt->getNumPlyrs(); j++) {
                    tempScores[j] = scores[j];
                }

                if (bidFlag) {
                    //std::cout << "In bidFlag loop #1" << std::endl;
                    int numCardsPlayed = gmSt->getNumCardsPlayed();
                    //std::cout << "numCardsPlayed = " << numCardsPlayed << std::endl;
                    for (int j = 0; j < gmSt->getNumPlyrs(); j++) {
                        //std::cout << "j = " << j << std::endl;
                        for (int k = 0; k < gmSt->getTotalCards() + 1; k++) {
                            if (i > 0) {
                                //std::cout << "About to access tempBonusProbs at [";
                                //std::cout << (numCardsPlayed - 1) * gmSt->getNumPlyrs() + j << "][";
                                //std::cout << k << "]" << std::endl;
                                tempBonusProbs[(numCardsPlayed - 1) * gmSt->getNumPlyrs() + j][k] = bonusProb[j][k];
                                //std::cout << "Done accessing tempBonusProbs" << std::endl;
                            } else {
                                //std::cout << "About to access tempBonusProbs at [";
                                //std::cout << (numCardsPlayed - 1) * gmSt->getNumPlyrs() + j << "][";
                                //std::cout << k << "]" << std::endl;
                                tempBonusProbs[(numCardsPlayed - 1) * gmSt->getNumPlyrs() + j][k] = 0;
                                //std::cout << "Done accessing tempBonusProbs" << std::endl;
                            }
                        }
                    }
                }

                Card * tempCardPlayed = simulatePlay(bidFlag); // RECURSIVE CALL FOR NEXT PLAYER
                delete tempCardPlayed; // Memory allocted in simulatePlay needs to be deleted

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

                    // avg scores from the plays that tie in expected points for player in question
                    for (int j = 0; j < gmSt->getNumPlyrs(); j++) {
                        scores[j] = (scores[j] + tieCount * tempScores[j]) / static_cast<double>(tieCount + 1);
                    }
                }


                if (bidFlag) {
                    //std::cout << "In bidFlag loop #2" << std::endl;
                    int numCardsPlayed = gmSt->getNumCardsPlayed();
                    // For the position in question (currPosition):
                    // If one option is clearly better than the other, then they take that and the entire 2D array becomes the one
                    // If there is a mix, then we take the average of the two 2d arrays
                    // Criteria for 'clearly better'
                    // 80% chance at 0 better than 50% chance at 1
                    // So, BID_CORRECT_BONUS * highest probability, whichever is greater
                    // If they tie, whichever is for the greater bid
                    // i.e. 50% chace at 1 better than 50% chance at 0
                    // If both of those tie, then we have a real tie and just average
                    double maxBonusProb = 0.0, maxTempBonusProb = 0.0;
                    int maxBonusProbIdx = 0, maxTempBonusProbIdx = 0;
                    for (int j = 0; j < gmSt->getTotalCards() + 1; j++) {
                        if (BID_CORRECT_BONUS * bonusProb[currPosition][j] >= maxBonusProb) {
                            maxBonusProb = BID_CORRECT_BONUS * bonusProb[currPosition][j];
                            maxBonusProbIdx = j;
                        }
                        if (BID_CORRECT_BONUS *
                            tempBonusProbs[(numCardsPlayed - 1) * gmSt->getNumPlyrs() + currPosition][j] >= maxTempBonusProb) {
                            maxTempBonusProb = BID_CORRECT_BONUS *
                                               tempBonusProbs[(numCardsPlayed - 1) * gmSt->getNumPlyrs() + currPosition][j];
                            maxTempBonusProbIdx = j;
                        }
                    }

                    // TODO: add in check for if probs are equal but one is for a higher bid
                    if (maxTempBonusProb > maxBonusProb) {
                        // copy temp into actual
                        for (int j = 0; j < gmSt->getNumPlyrs(); j++) {
                            for (int k = 0; k < gmSt->getTotalCards() + 1; k++) {
                                bonusProb[j][k] = tempBonusProbs[(numCardsPlayed - 1) * gmSt->getNumPlyrs() + j][k];
                            }
                        }
                    } else if (abs(maxTempBonusProb - maxBonusProb) < 0.001 && maxBonusProbIdx < maxTempBonusProbIdx){
                        // copy temp into actual
                        for (int j = 0; j < gmSt->getNumPlyrs(); j++) {
                            for (int k = 0; k < gmSt->getTotalCards() + 1; k++) {
                                bonusProb[j][k] = tempBonusProbs[(numCardsPlayed - 1) * gmSt->getNumPlyrs() + j][k];
                            }
                        }
                    } else if (abs(maxTempBonusProb - maxBonusProb) < 0.001 && maxBonusProbIdx == maxTempBonusProbIdx) {
                        // average
                        bidTieCount++;
                        for (int j = 0; j < gmSt->getNumPlyrs(); j++) {
                            for (int k = 0; k < gmSt->getTotalCards() + 1; k++) {
                                bonusProb[j][k] = (bonusProb[j][k] +
                                                   bidTieCount *
                                                   tempBonusProbs[(numCardsPlayed - 1) * gmSt->getNumPlyrs() + j][k])
                                                  / static_cast<double>(bidTieCount + 1);
                            }
                        }
                    }
                }


                gmSt->reversePlay();
            }
        }
	}

	//if (bidFlag && !gmSt->isLastTrick()){
    //    // delete tempBonusProb
    //    for (int i = 0; i < gmSt->getNumPlyrs(); i++){
    //        delete [] tempBonusProb[i];
    //    }
    //    delete [] tempBonusProb;
    //}

	// As cards are played and plays reversed, the order of cards in a player's hand is reversed
    // So, position of card i is at getCardsRemaining() - original position - 1
    return new Card(*(gmSt->getCardFromPlyrHands(gmSt->getNextToAct(), gmSt->getCardsRemaining() - currBestCard - 1)));
}



/**********************************************************************************************************************
 * GENERATING OPPONENT HANDS
 **********************************************************************************************************************/

/*
 * GENOPPONENTHANDS
 * Called from findBestBid or from simulatePlay to populate opponent hands with
 * random cards.
 * Game may be in progress, but genOpponentHands will only generate the number of
 * cards that are supposed to be remaining in a player's hand.
 * For example, if the player in 1st position has played a card then genOpponentHands is called,
 * That player's hand will be filled with one fewer card than all the others.
 */
bool DecisionPoint::genOpponentHands(){
    // Make gameState that is a reconstruction of the start of the current gmSt
    GameState * masterGmSt = reconstructGmStFromStart();
    //std::cout << "Reconstructed gameState" << std::endl;
    // For each player, produce a hand that matches their bid and copy it in to masterGmSt
    for (int i = 0; i < gmSt->getNumPlyrs(); i++) {
        fillPlayerHand(masterGmSt, i);
        //std::cout << "Through fillPlayerHand with i = " << i << std::endl;
    }
    //std::cout << "Through filling player hands" << std::endl;

    // Copy those generated hands back into the original gmSt
    copyOpponentHandsToGmSt(masterGmSt);

    delete masterGmSt;
    masterGmSt = nullptr;

    // Only change after this function has been run is that DecisionPoint's gmSt member object has all player
    // hands filled with cards that would have bid numbers that match what they actually bid in the game
    return true;
}

/*
 * FILLPLAYERHAND
 * Determines what hand generation algorithm must be used, based on whether player has
 * bid or not.
 */
void DecisionPoint::fillPlayerHand(GameState* masterGmSt, int plyrPosition){
    if (masterGmSt->getBid(plyrPosition) == -1){ // Player hasn't bid yet, so hand is totally random
        addRandomHand(masterGmSt, masterGmSt, plyrPosition);
    } else { // Player has bid, so their hand has to be one that would ooptimally bid whatever they bid
        //std::cout << "About to call addHandToMatchBid" << std::endl;
        addHandToMatchBid(masterGmSt, plyrPosition);
    }
}

/*
 * ADDHANDTOMATCHBID
 * Generate hand that matches bid already present in masterGmSt
 * i.e. make a hand where if a player were holding it,
 * they would optimally bid whatever they have actually bid.
 * Note: there is some extimation here for speed purposes. If a random
 * hand is generated that is within 2 of the actual player bid, then
 * the code loops a maximum of 10 more times to try to find a better (closer to
 * the actual bid) hand. If a hand that would bid within 1 of the actual bid
 * is found, a maximum of 5 more tries are done to try to find a better hand.
 * So, in many cases, the ultimate hand used will be 1 or 2 off of what the player actually bid.
 */
void DecisionPoint::addHandToMatchBid(GameState * masterGmSt, int plyrPosition){
    GameState *indivGmSt = nullptr;
    DecisionPoint *newDPoint = nullptr;
    int noMatchRemaining = 30;
    bool handGenerated = false, newHand = false;
    std::vector<std::string> tempBestHand;

    do {
        indivGmSt = setNewRandomHandGmSt(plyrPosition, masterGmSt);
        /*
        std::cout << "masterGmSt:" << std::endl;
        std::cout << "Bids: ";
        for (int i = 0; i < masterGmSt->getNumPlyrs(); i++){
            std::cout << masterGmSt->getBid(i) << ", ";
        }
        std::cout << std::endl;
        for (int i = 0; i < masterGmSt->getNumPlyrs(); i++){
            std::cout << "Player #" << i << ": ";
            for (int j = 0; j < masterGmSt->getTotalCards(); j++){
                if (masterGmSt->getCardFromPlyrHands(i, j) != nullptr){
                    std::cout << masterGmSt->getCardFromPlyrHands(i, j)->getCardStr() << " ";
                } else {
                    std::cout << "-- ";
                }
            }
            std::cout << std::endl;
        }

        std::cout << "indivGmSt:" << std::endl;
        std::cout << "Bids: ";
        for (int i = 0; i < indivGmSt->getNumPlyrs(); i++){
            std::cout << indivGmSt->getBid(i) << ", ";
        }
        std::cout << std::endl;
        for (int i = 0; i < indivGmSt->getNumPlyrs(); i++){
            std::cout << "Player #" << i << ": ";
            for (int j = 0; j < indivGmSt->getTotalCards(); j++){
                if (indivGmSt->getCardFromPlyrHands(i, j) != nullptr){
                    std::cout << indivGmSt->getCardFromPlyrHands(i, j)->getCardStr() << " ";
                } else {
                    std::cout << "-- ";
                }
            }
            std::cout << std::endl;
        }
         */


        // Make a decisionPoint with that and check that the bid from that player would be correct
        newDPoint = new DecisionPoint(indivGmSt);
        //int tempBidFind = newDPoint->findBestBid();
        //std::cout << "tempBidFind: " << tempBidFind << ", masterGmSt bid: " << masterGmSt->getBid(plyrPosition) << std::endl;
        //int bidDiff = abs(tempBidFind - masterGmSt->getBid(plyrPosition));
        //std::cout << "bidDiff: " << bidDiff << std::endl;
        int bidDiff = abs(newDPoint->findBestBid() - masterGmSt->getBid(plyrPosition));
        newHand = false;
        if (bidDiff == 0){
            handGenerated = true;
            newHand = true;
        } else if (bidDiff <= 2){
            noMatchRemaining--;
            noMatchRemaining = std::min(noMatchRemaining, bidDiff * 5);
            newHand = true;
        } else {
            if (noMatchRemaining == 30){
                newHand = true;
            }
            noMatchRemaining--;
        }
        // Save hand
        if (newHand) {
            tempBestHand.clear();
            for (int j = 0; j < indivGmSt->getTotalCards(); j++) {
                tempBestHand.push_back(indivGmSt->getCardFromPlyrHands(plyrPosition, j)->getCardStr());
            }
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

/*
 * SETNEWRANDOMHANDGMST
 * Utility function to build a gamestate with actual bida dn generated hands for players in front of
 * the one next to act and fully random hands for the player to act and anyone after the player.
 */
GameState* DecisionPoint::setNewRandomHandGmSt(int currPosition, GameState * masterGmSt){
    // Make a copy of masterGameState
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

    // For players in front of current player to act, set their bids
    for (int j = 0; j < currPosition; j++) {
        indivGmSt->makeBid(masterGmSt->getBid(j));
    }

    return indivGmSt;
}

/*
 * RECONSTRUCTGMSTFROMSTART
 * Utility function that takes a gamestate of a game in progress and makes
 * a gamestate that matches how the game in progress would have looked after bidding
 * but before any plays were made.
 */
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

/*
 * COPYOPPONENTHANDSTOGMST
 * Utility function to copy all hands except for hero's hand into a passed in gamestate
 */
void DecisionPoint::copyOpponentHandsToGmSt(GameState * masterGmSt){
    for (int i = 0; i < gmSt->getNumPlyrs(); i++){
        if (i != gmSt->getHeroPosition()){
            for (int j = 0; j < gmSt->getTotalCards(); j++){
                gmSt->addCardToPlyrHand(i, masterGmSt->getCardFromPlyrHands(i, j)->getCardStr());
            }
        }
    }
}

/*
 * ADDRANDOMHAND
 * Mark what suits are invalid based on current state of game, then generate
 * random suit (1 - 4) and random card value (1 - 13) until the random hand is one
 * that is a valid suit and hasn't been used in the gamestate already.
 * Add that hand to the GameState
 */
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
            indivGmSt->addCardToPlyrHand(position, cardVal, cardSuit);
        }
    }
}

/*
 * CARDALREADYUSED
 * Function to check if a card has been used in any of the three passed in gamestates.
 */
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

/*
 * MARKINVALIDSUITS
 * Function to mark suits that cannot be part of a player's generated random hand
 * The logic is that if a suit was led on a previous round and the player did not follow suit, then they cannot have any of that suit
 * So, the function checks the leading suit of each completed round, and if the player did not follow suit,
 * marks the leading suit of that round as invalid for card generation for future round simulation
 */
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

/*
 * ISVALIDSUIT
 * Returns whether the suit of a card is allowed per the specs of a validSuits array.
 */
bool DecisionPoint::isValidSuit(int cardSuit, bool * validSuits){
    if (validSuits[cardSuit - 1]){
        return true;
    }
    return false;
}