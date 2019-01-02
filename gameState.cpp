

#include "gameState.h"

GameState::GameState(int numPlyrs, int heroPosition, int totalCards, Card * flippedCard, Card ** heroHand) {
    this->numPlyrs = numPlyrs;
    bids = new int[numPlyrs] {-1};
    this->totalCards = totalCards;
    this->numCardsRemaining = totalCards; // Assuming gameplay has not started
    this->nextToAct = 0;
    this->heroPosition = heroPosition - 1; // 0-based rather than 1-based
    this->flippedCard = flippedCard;
    this->trump = flippedCard->getSuit();

    roundLead = new int[totalCards]();
    roundLead[0] = 0; // Position 1 starts 1st round

    plyrHands = new Card**[numPlyrs];
    this->plyrHands[heroPosition] = heroHand;
    for (int i = 0; i < numPlyrs; i++){
        if (i != heroPosition){
            plyrHands[i] = new Card*[numCardsRemaining];
        }
    }

    plydCrds = new Card**[numPlyrs];
    for (int i = 0; i < numPlyrs; i++){
        plydCrds[i] = new Card*[totalCards];
    }

}

GameState::GameState(const GameState &oldGmSt){
    this->numPlyrs = oldGmSt.numPlyrs;
    this->bids = new int[this->numPlyrs];
    for (int i = 0; i < this->numPlyrs; i++){
        this->bids[i] = oldGmSt.bids[i];
    }
    this->totalCards = oldGmSt.totalCards;
    this->numCardsRemaining = oldGmSt.numCardsRemaining;
    this->trump = oldGmSt.trump;

    // previous cards played

}

GameState::~GameState(){
    delete bids;
}


int GameState::getNumPlyrs(){
    return numPlyrs;
}

int GameState::getBid(int position){
    return bids[position - 1];
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

int GameState::getHeroPosition(){
    return heroPosition;
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

int GameState::getNextToAct(){
    return nextToAct;
}

void GameState::updateNextToAct(){
    //std::cout << "In 'GameState::updateNextToAct'" << std::endl;
    nextToAct = (nextToAct + 1) % numPlyrs;
    //std::cout << "Next to act (after updating - code version): " << nextToAct << std::endl;
    if (nextToAct == roundLead[totalCards - numCardsRemaining]){
        nextToAct = findTrickWinner(totalCards - numCardsRemaining);
        decCardsRemaining();
    }
}

void GameState::addCardPlayed(std::string card){
    //std::cout << "In 'GameState::addCardPlayed'" << std::endl;
    plydCrds[totalCards - numCardsRemaining][nextToAct] = new Card(card);
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

void GameState::removeCardFromHand(int plyrPosition, std::string card){
    for (int i = 0; i < totalCards; i++){
        if (plyrHands[plyrPosition][i] != nullptr && plyrHands[plyrPosition][i]->getCardStr() == card){
            delete plyrHands[plyrPosition][i];
            plyrHands[plyrPosition][i] = nullptr;
            break;
        }
    }
}