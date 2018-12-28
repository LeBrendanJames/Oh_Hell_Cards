

#include "gameState.h"

GameState::GameState(int numPlyrs, int totalCards, Suit trump) {
    this->numPlyrs = numPlyrs;
    bids = new int[numPlyrs];
    this->totalCards = totalCards;
    this->numCardsRemaining = totalCards; // Assuming gameplay has not started
    this->trump = trump;

    heroHand = new Card[totalCards];
}

GameState::~GameState(){

}