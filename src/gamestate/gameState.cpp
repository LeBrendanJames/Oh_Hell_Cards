#include "gameState.h"

GameState::GameState(int numPlyrs, int heroPosition, int totalCards, Card * flippedCard) {
    this->numPlyrs = numPlyrs;
    this->heroPosition = heroPosition;
    this->totalCards = totalCards;
    this->numCardsRemaining = totalCards; // Assuming gameplay has not started
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
    }

    plydCrds = new Card**[totalCards] {nullptr};
    for (int i = 0; i < totalCards; i++){
        plydCrds[i] = new Card*[numPlyrs] {nullptr};
    }

    this->flippedCard = new Card(*flippedCard);
}

GameState::GameState(int numPlyrs, int heroPosition, int totalCards, Card * flippedCard, Card ** heroHand) {
    this->numPlyrs = numPlyrs;
    this->heroPosition = heroPosition;
    this->totalCards = totalCards;
    this->numCardsRemaining = totalCards; // Assuming gameplay has not started
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
}

GameState::GameState(const GameState &oldGmSt){
    this->numPlyrs = oldGmSt.numPlyrs;
    this->heroPosition = oldGmSt.heroPosition;
    this->totalCards = oldGmSt.totalCards;
    this->numCardsRemaining = oldGmSt.numCardsRemaining;
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

/*********************************************************************************************************************
 * GETTERS
 *
 *********************************************************************************************************************/
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
    return flippedCard->getSuit();
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

int GameState::getNumCardsPlayed(){
    int numCards = 0;
    for (int i = 0; i < totalCards; i++){
        for (int j = 0; j < numPlyrs; j++){
            if (plydCrds[i][j] != nullptr){
                numCards++;
            }
        }
    }

    return numCards;
}

Card * GameState::getCardFromPlyrHands(int player, int cardPosition){
    return plyrHands[player][cardPosition];
}

Card * GameState::getCardFromPlydCrds(int round, int position){
    return plydCrds[round][position];
}

Card * GameState::getFlippedCard(){
    return flippedCard;
}


/**********************************************************************************************************************
 * SETTERS
 *
 *********************************************************************************************************************/
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

bool GameState::addCardToPlyrHand(int playerPos, int val, int suit){
    bool validCard = !cardPrevUsed(val, suit);

    bool cardAdded = false;
    int i = 0;
    while (validCard && !cardAdded && i < numCardsRemaining){
        if (plyrHands[playerPos][i] == nullptr){
            plyrHands[playerPos][i] = new Card(val, suit);
            cardAdded = true;
        }
        i++;
    }

    return validCard && cardAdded;
}



/**********************************************************************************************************************
 * GAMEPLAY PUBLIC FUNCTIONS
 *********************************************************************************************************************/
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

void GameState::reversePlay(){
    reverseNextToAct(); // Must be done first. Next two functions rely on this being already moved back one move.
    restoreCardToPlyrHand(nextToAct);
    removeCardFromPlydCrds(totalCards - numCardsRemaining, nextToAct); // **Previously passed currRound but theres a bug with updating that**
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


/**********************************************************************************************************************
 * QUERY STATE OF GAME
 *********************************************************************************************************************/
bool GameState::isTrump(Card * card){
    if (card->getSuit() == flippedCard->getSuit()){
        return true;
    } else {
        return false;
    }
}

bool GameState::cardPrevUsed(std::string card){
    Card cardToCheck(card);

    // Check if card in plyrHands or plydCrds
    int i = 0, j = 0, k = 0;
    while (i < numPlyrs){
        // Check against cards in player hands
        j = 0;
        while (j < numCardsRemaining){
            if (plyrHands[i][j] != nullptr && cardToCheck == *(plyrHands[i][j])) { // Order matters, null check first
                return true;
            }
            j++;
        }

        // Check against cards in plydCrds
        k = 0;
        while (k < totalCards){
            if (plydCrds[k][i] != nullptr && cardToCheck == *(plydCrds[k][i])){ // Order matters, null check first
                return true;
            }
            k++;
        }

        i++;
    }

    // Check against flipped card
    if (cardToCheck == *flippedCard){
        return true;
    }

    return false;
}

bool GameState::cardPrevUsed(int cardVal, int cardSuit){
    // Check if card in plyrHands or plydCrds
    int i = 0, j = 0, k = 0;
    while (i < numPlyrs){
        // Check against cards in player hands
        j = 0;
        //while (j < totalCards){
        while (j < numCardsRemaining){
            if (plyrHands[i][j] != nullptr &&
                cardVal == plyrHands[i][j]->getVal() &&
                cardSuit == static_cast<int>(plyrHands[i][j]->getSuit()) + 1){
                return true;
            }
            j++;
        }

        // Check against cards in plydCrds
        k = 0;
        while (k < totalCards){
            if (plydCrds[k][i] != nullptr &&
                cardVal == plydCrds[k][i]->getVal() &&
                cardSuit == static_cast<int>(plydCrds[k][i]->getSuit()) + 1){
                return true;
            }
            k++;
        }

        i++;
    }

    // Check against flipped card
    if (cardVal == flippedCard->getVal() && cardSuit == static_cast<int>(flippedCard->getSuit()) + 1){
        return true;
    }

    return false;
}

bool GameState::allHandsGenerated(){
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

    if (cardCount == numPlyrs * totalCards){
        return true;
    } else {
        return false;
    }
}

bool GameState::isLastTrick() {
    return roundLead[totalCards - 1] != -1;
}


/**********************************************************************************************************************
 * PRIVATE GAMEPLAY HELPER FUNCTIONS
 *********************************************************************************************************************/
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
            while (i < numCardsRemaining && valid){
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

bool GameState::removeCardFromPlydCrds(int round, int position){
    delete plydCrds[round][position];
    plydCrds[round][position] = nullptr;
    return true;
}

bool GameState::restoreCardToPlyrHand(int plyrPosition){
    for (int i = numCardsRemaining - 1; i > 0; i--){
        // Move cards out one slot so that 1st slot is open fro card to be added back in
        plyrHands[plyrPosition][i] = plyrHands[plyrPosition][i - 1];
    }

    // Add card back in
    plyrHands[plyrPosition][0] = new Card(*(plydCrds[totalCards - numCardsRemaining][plyrPosition]));

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

void GameState::reverseNextToAct(){
    if (plydCrds[0][0] == nullptr){ // In bid phase (either still bidding or everyone has bid but nobody has played card yet)
        // Adding numPlyrs then mod math just to wrap from 0 to numPlyrs - 1 correctly
        nextToAct = (nextToAct + numPlyrs - 1) % numPlyrs;
    } else if (nextToAct == roundLead[totalCards - numCardsRemaining]) { // Ended round and set up next
        nextToAct = (roundLead[totalCards - numCardsRemaining - 1] + numPlyrs - 1) % numPlyrs;
        roundLead[totalCards - numCardsRemaining] = -1;
        numCardsRemaining++;
    } else if (nextToAct == -1){ // Game ended w/ previous move
        nextToAct = (roundLead[totalCards - 1] + numPlyrs - 1) % numPlyrs;
        numCardsRemaining++;
    } else { // Just within a round (moving back from plyr 3 to plyr 2, for example)
        nextToAct = (nextToAct + numPlyrs - 1) % numPlyrs;
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