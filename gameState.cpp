

#include "gameState.h"

GameState::GameState(int numPlyrs, int heroPosition, int totalCards, Card * flippedCard, Card ** heroHand) {
    this->numPlyrs = numPlyrs;
    this->heroPosition = heroPosition - 1; // 0-based rather than 1-based
    this->totalCards = totalCards;
    this->numCardsRemaining = totalCards; // Assuming gameplay has not started
    this->trump = flippedCard->getSuit();
    this->nextToAct = 0;
    bids = new int[numPlyrs] {-1};
    roundLead = new int[totalCards]{-1};
    roundLead[0] = 0; // Position 0 starts 1st round

    plyrHands = new Card**[numPlyrs];
    this->plyrHands[heroPosition] = heroHand;
    for (int i = 0; i < numPlyrs; i++){
        if (i != heroPosition){
            plyrHands[i] = new Card*[numCardsRemaining];
        }
    }

    plydCrds = new Card**[totalCards];
    for (int i = 0; i < totalCards; i++){
        plydCrds[i] = new Card*[numPlyrs];
    }

    this->flippedCard = flippedCard;
}

GameState::GameState(const GameState &oldGmSt){
    this->numPlyrs = oldGmSt.numPlyrs;
    this->heroPosition = oldGmSt.heroPosition;
    this->totalCards = oldGmSt.totalCards;
    this->numCardsRemaining = oldGmSt.numCardsRemaining;
    this->trump = oldGmSt.trump;
    this->nextToAct = oldGmSt.nextToAct;

    this->bids = new int[this->numPlyrs];
    for (int i = 0; i < this->numPlyrs; i++){
        this->bids[i] = oldGmSt.bids[i];
    }

    this->roundLead = new int[this->totalCards];
    for (int i = 0; i < this->totalCards; i++){
        this->roundLead[i] = oldGmSt.roundLead[i];
    }

    this->plyrHands = new Card**[this->numPlyrs];
    for (int i = 0; i < this->totalCards; i++){
        this->plyrHands[i] = new Card*[this->numCardsRemaining];
        for (int j = 0; j < this->numCardsRemaining; j++){
            this->plyrHands[i][j] = new Card(*(oldGmSt.plyrHands[i][j]));
        }
    }

    this->plydCrds = new Card**[this->totalCards];
    for (int i = 0; i < this->totalCards; i++){
        this->plydCrds[i] = new Card*[this->numPlyrs];
        for (int j = 0; j < this->numPlyrs; j++){
            this->plydCrds[i][j] = new Card(*(oldGmSt.plydCrds[i][j]));
        }
    }

    this->flippedCard = new Card(*(oldGmSt.flippedCard));
}

GameState::~GameState(){
    delete [] bids;
    delete [] roundLead;

    for (int i = 0; i < numPlyrs; i++){
        for (int j = 0; j < totalCards; j++){
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
    return bids[position - 1];
}

int GameState::getRoundLead(int roundNum){
    return roundLead[roundNum];
}

void GameState::setBid(int position, int bid) {
    bids[position - 1] = bid;
}

bool GameState::isTrump(Card * card){
    if (card->getSuit() == trump){
        return true;
    } else {
        return false;
    }
}

void GameState::decCardsRemaining(){
    numCardsRemaining--;
}

void GameState::genOpponentHands() {
	int cardVal = -1, cardSuit = -1;
	bool cardAlreadyUsed = true;
	
	for (int i = 0; i < numPlyrs; i++){
		if (i != heroPosition){
			for (int j = 0; j < totalCards; j++){
				// Generate random card, making sure it hasn't already been used
				do {
					cardVal = rand() % 13 + 1;
					cardSuit = rand() % 4;
					cardAlreadyUsed = checkCardAlreadyUsed(cardVal, cardSuit);
				} while (cardAlreadyUsed);
				
				// Add to plyrHands array 
				plyrHands[i][j] = new Card(cardVal, cardSuit);
			}
		}
	}
}

bool GameState::cardAlreadyUsed(int cardVal, int cardSuit){
	Card cardToCheck(cardVal, cardSuit);
	
	// Check if card in plyrHands or plydCrds
	int i = 0, j = 0;
	while (i < numPlyrs && match = false){
		j = 0;
		while (j < totalCards && match = false){
			if (cardToCheck = *(plyrHands[i][j]) && cardToCheck == *(plydCrds[i][j])){
				match = true;
			}
			j++;
		}
		i++;
	}
	
	// Check against flipped card
	if (match == false && cardToCheck == *flippedCard){
		match = true;
	}

	return match;
}

bool GameState::playCard(int cardToPlay){
    // 1. Check if play is valid
	bool isValidPlay = checkValidPlay(heroPosition, cardToPlay); 
    
    // If it is:
    if (isValidPlay) {
        // 2. Add card to playedCards 2d arrays
        addCardPlayed(plyrHands[heroPosition][cardToPlay]->getCardStr());
        // 3. Remove card from players hand (and move the rest up? That's slow w/ an array so this should probably be a vector. Can then just swap to end and pop_back.)
        removeCardFromHand(heroPosition, plyrHands[heroPosition][cardToPlay]->getCardStr());
        // 4. Call updateNextToPlay
        updateNextToAct();
        // 5. return true
        return true;
    } else {
        // Else return false
        return false;
    }
}

bool GameState::checkValidPlay(int position, int cardToPlay){
	if (roundLead(currRound) == -1){
		return true; // Play is automatically valid if it is leading the round 
	} else {
		bool valid = true; // default to true unless below check isn't satisfied
		
		// find roundLead suit
		Suit leadSuit = plydCrds[currRound][roundLead(currRound)]->getSuit();
		
		// check if player in 'position' has any of that suit
		for (int i = 0; i < totalCards; i++){
			if (plyrHands[position][i] != nullptr && plyrHands[position][i]->getSuit == leadSuit && plyrHands[position][cardToPlay]->getSuit() != leadSuit){
				// player has suit to follow lead but isn't playing it = NOT VALID
				valid = false;
			}
		}
		
		return valid;
	}
	
}

void GameState::addCardPlayed(std::string card){
    //std::cout << "In 'GameState::addCardPlayed'" << std::endl;
    plydCrds[totalCards - numCardsRemaining][nextToAct] = new Card(card);
}


void GameState::removeCardFromHand(int plyrPosition, std::string card){
    for (int i = 0; i < totalCards; i++){
        if (plyrHands[plyrPosition][i] != nullptr && plyrHands[plyrPosition][i]->getCardStr() == card){
            delete plyrHands[plyrPosition][i];
            plyrHands[plyrPosition][i] = nullptr;
            break;
        }
    }
}

void GameState::updateNextToAct(){
    //std::cout << "In 'GameState::updateNextToAct'" << std::endl;
    nextToAct = (nextToAct + 1) % numPlyrs;
    //std::cout << "Next to act (after updating - code version): " << nextToAct << std::endl;
	if (nextToAct == roundLead[totalCards - numCardsRemaining]){ // Trick over 
        nextToAct = findTrickWinner(totalCards - numCardsRemaining);
        decCardsRemaining();
		if (numCardsRemaining != 0){ // If there are remaining tricks
			// Update roundLead array
			roundLead(totalCards - numCardsRemaining) = nextToAct;
		} else { // Update next to act to -1 if round/tricks are over 
			nextToAct = -1;
		}
    }
}

int GameState::findTrickWinner(int trickNum){
    //std::cout << "In 'GameState::findTrickWinner'" << std::endl;
    //std::cout << "trickNum: " << trickNum << std::endl;
    Card * currWinningCard = plydCrds[trickNum][roundLead[trickNum]];
    int currWinningPosition = roundLead[trickNum];
    int comparePosition = currWinningPosition + 1;
    //std::cout << "currWinningCard: " << currWinningCard->getCardStr() << std::endl;
    //std::cout << "currWinningPosition (computer version): " << currWinningPosition << std::endl;

    for (int i = 1; i < numPlyrs; i++) { // Loop through other players, checking if their card is in the leads
        if (isTrump(currWinningCard)){
            //std::cout << "Current winning card is trump." << std::endl;
            if (isTrump(plydCrds[trickNum][comparePosition])){
                //std::cout << "Comparison card is trump." << std::endl;
                // compare values
                if (*(plydCrds[trickNum][comparePosition]) > (*currWinningCard)){
                    // make new card winning card
                    currWinningCard = plydCrds[trickNum][comparePosition];
                    currWinningPosition = comparePosition;
                    //std::cout << "Updated currWinningPosition to: " << currWinningPosition << std::endl;
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