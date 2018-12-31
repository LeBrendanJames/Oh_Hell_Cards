

#include "gameState.h"

GameState::GameState(int numPlyrs, int totalCards, Suit trump) {
    this->numPlyrs = numPlyrs;
    bids = new int[numPlyrs] {-1};
    this->totalCards = totalCards;
    this->numCardsRemaining = totalCards; // Assuming gameplay has not started
    this->trump = trump;
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
    return bids[position];
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

void GameState::setBid(int position, int bid) {
    bids[position] = bid;
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