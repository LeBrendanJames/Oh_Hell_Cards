

#include "gameState.h"

GameState::GameState(int numPlyrs, int heroPosition, int totalCards, Card * flippedCard, Card ** heroHand) {
    this->numPlyrs = numPlyrs;
    this->heroPosition = heroPosition;
    this->totalCards = totalCards;
    this->numCardsRemaining = totalCards; // Assuming gameplay has not started
    this->trump = flippedCard->getSuit();
    this->nextToAct = 0;
    this->currRound = 0;
    bids = new int[numPlyrs];
    for (int i = 0; i < numPlyrs; i++){
        bids[i] = -1;
    }

    roundLead = new int[totalCards];
    for (int i = 0; i < totalCards; i++){
        roundLead[i] = -1;
    }
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

    for (int i = 0; i < numPlyrs; i++){
        for (int j = 0; j < numCardsRemaining; j++){
            delete plyrHands[i][j];
        }
        delete [] plyrHands[i];
    }
    delete [] plyrHands;

    for (int i = 0; i < totalCards; i++){
        for (int j = 0; j < numPlyrs; j++){
            delete plydCrds[i][j];
        }
        delete [] plydCrds[i];
    }
    delete [] plydCrds;

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

bool GameState::makeBid(int bid){
	if (bid >= 0 && bid <= totalCards){
		bids[nextToAct] = bid;
		updateNextToAct();
		return true;
	}
	
	return false;
}

bool GameState::playCard(int cardToPlay){
    bool isValidPlay = checkValidPlay(nextToAct, cardToPlay);

    if (isValidPlay) {
        addCardToPlydCrds(currRound, nextToAct, plyrHands[nextToAct][cardToPlay]->getCardStr());

        removeCardFromPlyrHand(nextToAct, plyrHands[nextToAct][cardToPlay]->getCardStr());

        updateNextToAct();

        return true;
    } else {
        return false;
    }
}

bool GameState::calcFinalScores(){
    if (nextToAct != -1){
        return false; // Game not finished
    }

    for (int i = 0; i < numPlyrs; i++){
        // Set to 0 points rather than -1, since each player starts on 0
        // -1 was just an indicator that the round had not finished so final scores couldn't be calculated
        finalScores[i] = 0;
    }

    for (int i = 0; i < totalCards; i++){ // Loop through rounds
        int currWinningPosition = findTrickWinner(i);

        // Add 1 to score of winning position
        finalScores[currWinningPosition]++;
    }

    // check scores of each position versus their bids. Add bonus to score if they match.
    for (int i = 0; i < numPlyrs; i++){
        if (bids[i] == finalScores[i]){
            finalScores[i] += BID_CORRECT_BONUS;
        }
    }

    return true;
}

bool GameState::allHandsGenerated(){
    //std::cout << "In GameState::allHandsGenerated()" << std::endl;
    int cardCount = 0, cardCountFromHands = 0, cardCountFromPlyd = 0;

    for (int i = 0; i < numPlyrs; i++){
        for (int j = 0; j < numCardsRemaining; j++){
            if (plyrHands[i][j] != nullptr){
                cardCount++;
                cardCountFromHands++;
            }

        }
        for (int j = 0; j < totalCards; j++){
            if (plydCrds[j][i] != nullptr){
                cardCount++;
                cardCountFromPlyd++;
            }
        }
    }

    //std::cout << "cardCount = " << cardCount << std::endl;
    //std::cout << "cardCountFromHands = " << cardCountFromHands << std::endl;
    //std::cout << "cardCountFromPlyd = " << cardCountFromPlyd << std::endl;
    //std::cout << "numPlyrs * totalCards = " << numPlyrs * totalCards << std::endl;

    if (cardCount == numPlyrs * totalCards){
        //std::cout << "Returning true" << std::endl;
        return true;
    } else {
        //std::cout << "Returning false" << std::endl;
        return false;
    }
}

bool GameState::cardPrevUsed(std::string card){
	Card cardToCheck(card);
	bool match = false;
	
	// Check if card in plyrHands or plydCrds
    int i = 0, j = 0, k = 0;
    while (i < numPlyrs && !match){
        // Check against cards in player hands
        j = 0;
        while (j < numCardsRemaining && !match){
            if (plyrHands[i][j] != nullptr && cardToCheck == *(plyrHands[i][j])) { // Order matters, null check first
                match = true;
            }
            j++;
        }

        // Check against cards in plydCrds
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
    if (cardToPlay < 0 || cardToPlay >= numCardsRemaining){
        return false; // If cardToPlay outside range
    } else if (plyrHands[position][cardToPlay] == nullptr){
        return false; // If there isn't a card there to play
    } else if ((roundLead[currRound] == -1 || roundLead[currRound] == position)){
		return true; // Play is automatically valid if it is leading the round 
	} else {
		bool valid = true; // default to true unless below check isn't satisfied

		// find roundLead suit
		Suit leadSuit = plydCrds[currRound][roundLead[currRound]]->getSuit();

		// Find if player is not playing leadSuit
        if (plyrHands[position][cardToPlay]->getSuit() != leadSuit){
            int i = 0;
            // Check all other cards in player hand to see if there is one that matches leadSuit
            while (i < numCardsRemaining && valid == true){
                if (plyrHands[position][i] != nullptr && plyrHands[position][i]->getSuit() == leadSuit){
                    valid = false;
                }
                i++;
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
            // Remove card
            delete plyrHands[plyrPosition][i];
            plyrHands[plyrPosition][i] = nullptr;

            // Move remaining cards down in array
            int j = i + 1;
            while (j < numCardsRemaining && plyrHands[plyrPosition][j] != nullptr){
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

    if (plydCrds[0][0] == nullptr){
        // Means we're just finishing bid phase
        return;
    } else if (nextToAct == roundLead[totalCards - numCardsRemaining]){ // Trick over
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
    // Start with leading card winning trick
    Card * currWinningCard = plydCrds[trickNum][roundLead[trickNum]];
    int currWinningPosition = roundLead[trickNum];

    int comparePosition = (currWinningPosition + 1) % numPlyrs;

    // Loop through other players, checking if their card beats the current winning card
    while (comparePosition != roundLead[trickNum]){
        // If suits match
        if (plydCrds[trickNum][comparePosition]->getSuit() == currWinningCard->getSuit()){
            // if newCard > oldCard
            if (*(plydCrds[trickNum][comparePosition]) > *currWinningCard){
                currWinningCard = plydCrds[trickNum][comparePosition];
                currWinningPosition = comparePosition;
            }
        // else if new card is trump and current winning card not trump
        } else if (isTrump(plydCrds[trickNum][comparePosition]) && !isTrump(currWinningCard)){

            currWinningCard = plydCrds[trickNum][comparePosition];
            currWinningPosition = comparePosition;
        }

        // Finding next position to compare, looping back to position 0 with the % math.
        comparePosition = (comparePosition + 1) % numPlyrs;
    }

    return currWinningPosition;
}