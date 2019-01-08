

#include "gameState.h"

GameState::GameState(int numPlyrs, int heroPosition, int totalCards, Card * flippedCard, Card ** heroHand) {
    this->numPlyrs = numPlyrs;
    this->heroPosition = heroPosition;
    this->totalCards = totalCards;
    this->numCardsRemaining = totalCards; // Assuming gameplay has not started
    this->trump = flippedCard->getSuit();
    this->nextToAct = 0;
    this->currRound = 0;
    bids = new int[numPlyrs] {-1};

    roundLead = new int[totalCards]{-1};
    roundLead[0] = 0; // Position 0 starts 1st round

    finalScores = new int[numPlyrs] {-1};

    plyrHands = new Card**[numPlyrs] {nullptr};

    for (int i = 0; i < numPlyrs; i++){
        plyrHands[i] = new Card*[numCardsRemaining] {nullptr};
        if (i == this->heroPosition){ // If on hero hand, copy from passed in hand
            for (int j = 0; j < numCardsRemaining; j++) {
                this->plyrHands[i][j] = new Card(*(heroHand[j]));
            }
        }
    }

    plydCrds = new Card**[totalCards] {nullptr};
    for (int i = 0; i < totalCards; i++){
        plydCrds[i] = new Card*[numPlyrs] {nullptr};
    }

    this->flippedCard = new Card(*flippedCard);

    srand(time(NULL));
}

GameState::GameState(const GameState &oldGmSt){
    this->numPlyrs = oldGmSt.numPlyrs;
    this->heroPosition = oldGmSt.heroPosition;
    this->totalCards = oldGmSt.totalCards;
    this->numCardsRemaining = oldGmSt.numCardsRemaining;
    this->trump = oldGmSt.trump;
    this->nextToAct = oldGmSt.nextToAct;
    this->currRound = oldGmSt.currRound;

    this->bids = new int[this->numPlyrs];
    for (int i = 0; i < this->numPlyrs; i++){
        this->bids[i] = oldGmSt.bids[i];
    }

    this->roundLead = new int[this->totalCards];
    for (int i = 0; i < this->totalCards; i++){
        this->roundLead[i] = oldGmSt.roundLead[i];
    }

    this->finalScores = new int[this->numPlyrs];
    for (int i = 0; i < numPlyrs; i++){
        this->finalScores[i] = oldGmSt.finalScores[i];
    }

    this->plyrHands = new Card**[this->numPlyrs];
    for (int i = 0; i < this->numPlyrs; i++){
        this->plyrHands[i] = new Card*[this->numCardsRemaining];
        for (int j = 0; j < this->numCardsRemaining; j++){
            if (oldGmSt.plyrHands[i][j] != nullptr) {
                this->plyrHands[i][j] = new Card(*(oldGmSt.plyrHands[i][j]));
            } else {
                this->plyrHands[i][j] = nullptr;

            }
        }
    }

    this->plydCrds = new Card**[this->totalCards];
    for (int i = 0; i < this->totalCards; i++){
        this->plydCrds[i] = new Card*[this->numPlyrs];
        for (int j = 0; j < this->numPlyrs; j++){
            if (oldGmSt.plydCrds[i][j] != nullptr) {
                this->plydCrds[i][j] = new Card(*(oldGmSt.plydCrds[i][j]));
            } else {
                this->plydCrds[i][j] = nullptr;
            }
        }
    }

    this->flippedCard = new Card(*(oldGmSt.flippedCard));
}

GameState::~GameState(){
    delete [] bids;
    delete [] roundLead;
    delete [] finalScores;

    //std::cout << "Deleted simple arrays" << std::endl;


    for (int i = 0; i < numPlyrs; i++){
        //std::cout << "In 1st loop" << std::endl;
        for (int j = 0; j < numCardsRemaining; j++){
            //std::cout << "In 2nd loop" << std::endl;
            delete plyrHands[i][j];
        }
        delete [] plyrHands[i];
    }
    delete [] plyrHands;

    //std::cout << "Deleted plyrHands" << std::endl;

    for (int i = 0; i < totalCards; i++){
        for (int j = 0; j < numPlyrs; j++){
            delete plydCrds[i][j];
        }
        delete [] plydCrds[i];
    }
    delete [] plydCrds;

    //std::cout << "Deleted plydCrds" << std::endl;

    delete flippedCard;
}


int GameState::getNumPlyrs(){
    return numPlyrs;
}

int GameState::getHeroPosition(){
    return heroPosition;
}

int GameState::getTotalCards(){
    return totalCards;
}

int GameState::getCardsRemaining(){
    return numCardsRemaining;
}

Suit GameState::getTrump(){
    return trump;
}

int GameState::getNextToAct(){
    return nextToAct;
}

int GameState::getCurrRound(){
	return currRound;
}

int GameState::getBid(int position){
    return bids[position];
}

int GameState::getRoundLead(int roundNum){
    return roundLead[roundNum];
}

int GameState::getFinalScore(int plyrPosiiton){
    return finalScores[plyrPosiiton];
}

Card * GameState::getCardFromPlyrHands(int player, int cardPosition){
    return plyrHands[player][cardPosition];
}

Card * GameState::getCardFromPlydCrds(int round, int position){
    return plydCrds[round][position];
}

void GameState::setBid(int position, int bid) {
    bids[position] = bid;
}

// So that any object using GameState can fill player or opponent hands (whatever they need)
bool GameState::addCardToPlyrHand(int playerPos, std::string cardToAdd) {
    bool validCard = !cardPrevUsed(cardToAdd);

    bool cardAdded = false;
    int i = 0;
    while (validCard && !cardAdded && i < numCardsRemaining){
        if (plyrHands[playerPos][i] == nullptr){
            plyrHands[playerPos][i] = new Card(cardToAdd);
            cardAdded = true;
        }
        i++;
    }

    return validCard && cardAdded;
}

bool GameState::isTrump(Card * card){
    if (card->getSuit() == trump){
        return true;
    } else {
        return false;
    }
}

bool GameState::playCard(int cardToPlay){
    //std::cout << "About to checkValidPlay" << std::endl;
    //std::cout << "nextToAct = " << nextToAct << std::endl;
    //std::cout << "cardToPlay (int) = " << cardToPlay << std::endl;
    bool isValidPlay = checkValidPlay(nextToAct, cardToPlay);

    //std::cout << "Returned from checkValidPlay. isValidPlay = " << isValidPlay << std::endl;

    if (isValidPlay) {
        //std::cout << "About to addCardToPlydCrds" << std::endl;
        addCardToPlydCrds(currRound, nextToAct, plyrHands[nextToAct][cardToPlay]->getCardStr());
        //std::cout << "Done adding card to plydCrds" << std::endl;
        //std::cout << "About to removeCardFromPlyrHand" << std::endl;
        removeCardFromPlyrHand(nextToAct, plyrHands[nextToAct][cardToPlay]->getCardStr());
        //std::cout << "Done with removeCardFromPlyrHand" << std::endl;

        //std::cout << "About to updateNextToAct" << std::endl;
        updateNextToAct();
        //std::cout << "Done with updateNextToAct" << std::endl;

        return true;
    } else {
        return false;
    }
}

bool GameState::calcFinalScores(){
    if (nextToAct != -1){
        return false; // Game not finished
    } else {
        // Can instead be done with memset?
        for (int i = 0; i < numPlyrs; i++){
            finalScores[i] = 0;
        }

        // GameState will be full with all bids and cards played,
        // so should be a pretty simple tally of each trick then bonuses for tricks won = bid
        for (int i = 0; i < totalCards; i++){ // Loop through rounds
            int firstPosition = roundLead[i];

            Card currWinningCard = *(plydCrds[i][firstPosition]);
            int currWinningPosition = firstPosition;

            int comparePosition = (firstPosition + 1) % numPlyrs;

            for (int j = 1; j < numPlyrs; j++) { // Loop through other players, checking if their card is in the leads
                if (isTrump(&currWinningCard)){
                    if (isTrump(plydCrds[i][comparePosition])){
                        // compare values
                        if (*(plydCrds[i][comparePosition]) > currWinningCard){
                        //if (plydCrds[i][comparePosition]->getVal() > currWinningCard.getVal()){
                            // make new card winning card
                            currWinningCard = *(plydCrds[i][comparePosition]);
                            currWinningPosition = comparePosition;
                        }
                    }
                } else if (isTrump(plydCrds[i][comparePosition])){
                    // Make new card the winning card
                    currWinningCard = *(plydCrds[i][comparePosition]);
                    currWinningPosition = comparePosition;
                } else if (plydCrds[i][comparePosition]->getVal() > currWinningCard.getVal()){
                    // Make new card the winning card
                    currWinningCard = *(plydCrds[i][comparePosition]);
                    currWinningPosition = comparePosition;

                }

                comparePosition = (comparePosition + 1) % numPlyrs; // Finding next position, looping back to position 0 with the % math.
            }

            // Add 1 to score of winning position
            if (finalScores[currWinningPosition] == -1){
                finalScores[currWinningPosition] = 1;
            } else {
                finalScores[currWinningPosition]++;
            }
        }

        // check scores of each position versus their bids. Add 10 to score if they match.
        for (int i = 0; i < numPlyrs; i++){
            if (bids[i] == finalScores[i]){
                finalScores[i] += BID_CORRECT_BONUS;
            }
        }

        return true;
    }
}

bool GameState::cardPrevUsed(std::string card){
	Card cardToCheck(card);
	bool match = false;
	
	// Check if card in plyrHands or plydCrds
    int i = 0, j = 0, k = 0;
    while (i < numPlyrs && !match){
        j = 0;
        while (j < numCardsRemaining && !match){
            if (plyrHands[i][j] != nullptr && cardToCheck == *(plyrHands[i][j])) { // Order matters, null check first
                match = true;
            }
            j++;
        }

        k = 0;
        while (k < totalCards && !match){
            if (plydCrds[k][i] != nullptr && cardToCheck == *(plydCrds[k][i])){ // Order matters, null check first
                match = true;
            }
            k++;
        }

        i++;
    }
	
	// Check against flipped card
	if (!match && cardToCheck == *flippedCard){
		match = true;
	}

	return match;
}



bool GameState::checkValidPlay(int position, int cardToPlay){
    //std::cout << "In checkValidPlay" << std::endl;
    //std::cout << "numCardsRemaining = " << numCardsRemaining << std::endl;
    //std::cout << "currRound = " << currRound << std::endl;
    //std::cout << "roundLead[currRound] = " << roundLead[currRound] << std::endl;
    //if (cardToPlay >= 0 && cardToPlay < numCardsRemaining) {
    //    std::cout << "plyrHands[position][cardToPlay]->getCardStr() = "
    //                 << plyrHands[position][cardToPlay]->getCardStr()
    //              << std::endl;
    //}

    if (cardToPlay < 0 || cardToPlay >= numCardsRemaining){
        return false; // If cardToPlay outside range
    } else if (plyrHands[position][cardToPlay] == nullptr){
        return false; // If there isn't a card there to play
    } else if ((roundLead[currRound] == -1 || roundLead[currRound] == position)){
		return true; // Play is automatically valid if it is leading the round 
	} else {
        //std::cout << "In final 'else' stmt" << std::endl;
		bool valid = true; // default to true unless below check isn't satisfied

        //std::cout << "About to get leadSuit" << std::endl;
		// find roundLead suit
		Suit leadSuit = plydCrds[currRound][roundLead[currRound]]->getSuit();

		//std::cout << "Done getting leadSuit" << std::endl;
		
		// check if player in 'position' has any of that suit
		for (int i = 0; i < numCardsRemaining; i++){
		    //std::cout << "In loop" << std::endl;
			if (plyrHands[position][i] != nullptr &&
                plyrHands[position][i]->getSuit() == leadSuit &&
                plyrHands[position][cardToPlay]->getSuit() != leadSuit){

			    // player has suit to follow lead but isn't playing it = NOT VALID
				valid = false;
			}
		}

		return valid;
	}
	
}

bool GameState::addCardToPlydCrds(int round, int position, std::string card) {
    // TODO: Error checking?
    plydCrds[totalCards - numCardsRemaining][nextToAct] = new Card(card);

    return true;
}


bool GameState::removeCardFromPlyrHand(int plyrPosition, std::string card){
    for (int i = 0; i < numCardsRemaining; i++){
        if (plyrHands[plyrPosition][i] != nullptr && plyrHands[plyrPosition][i]->getCardStr() == card){
            delete plyrHands[plyrPosition][i];
            plyrHands[plyrPosition][i] = nullptr;

            int j = i + 1;
            while (j < numCardsRemaining && plyrHands[plyrPosition][j] != nullptr){
                // Move cards down
                plyrHands[plyrPosition][j - 1] = plyrHands[plyrPosition][j];
                j++; // move to next card
            }
            plyrHands[plyrPosition][j - 1] = nullptr;

            break;
        }
    }

    return true;
}

void GameState::updateNextToAct(){
    nextToAct = (nextToAct + 1) % numPlyrs;

	if (nextToAct == roundLead[totalCards - numCardsRemaining]){ // Trick over 
        nextToAct = findTrickWinner(totalCards - numCardsRemaining);
        numCardsRemaining--;
		if (numCardsRemaining != 0){ // If there are remaining tricks
			// Update roundLead array
			roundLead[totalCards - numCardsRemaining] = nextToAct;
		} else { // Update next to act to -1 if round/tricks are over 
			nextToAct = -1;
		}
    }
}

int GameState::findTrickWinner(int trickNum){
    Card * currWinningCard = plydCrds[trickNum][roundLead[trickNum]];
    int currWinningPosition = roundLead[trickNum];

    int comparePosition = (currWinningPosition + 1) % numPlyrs;

    for (int i = 1; i < numPlyrs; i++) { // Loop through other players, checking if their card is in the leads
        if (isTrump(currWinningCard)){
            if (isTrump(plydCrds[trickNum][comparePosition])){
                // compare values
                if (*(plydCrds[trickNum][comparePosition]) > (*currWinningCard)){
                    // make new card winning card
                    currWinningCard = plydCrds[trickNum][comparePosition];
                    currWinningPosition = comparePosition;
                }
            }
        } else if (isTrump(plydCrds[trickNum][comparePosition])){
            // Make new card the winning card
            currWinningCard = plydCrds[trickNum][comparePosition];
            currWinningPosition = comparePosition;
        } else if (*(plydCrds[trickNum][comparePosition]) > (*currWinningCard)){
            // Make new card the winning card
            currWinningCard = plydCrds[trickNum][comparePosition];
            currWinningPosition = comparePosition;
        }

        comparePosition = (comparePosition + 1) % numPlyrs; // Finding next position, looping back to position 0 with the % math.
    }

    return currWinningPosition;
}