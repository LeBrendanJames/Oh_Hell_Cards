

#include "Game.h"


Game::Game(int numPlayers, int heroPosition, int totalCards, Card * flippedCard, Card ** heroHand) {
    this->gmState = new GameState(numPlayers, heroPosition, totalCards, flippedCard, heroHand);
}

Game::Game(GameState * state){
    this->gmState = new GameState(*state);
}

Game::~Game(){
    delete gmState;
}

int Game::getBid(int position){
    return this->gmState->getBid(position);
}

void Game::simRound() {
    int plyrBid;
    std::string cardPlayed;


    while(gmState->getBid(gmState->getNextToAct()) == -1){
        if (gmState->getNextToAct() == gmState->getHeroPosition()){
            DecisionPoint * dPoint = new DecisionPoint(gmState);
            int bidRec = dPoint->recommendBid();
            std::cout << std::endl;
            std::cout << "Recommended bid for hero: " << bidRec << std::endl;
            std::cout << std::endl;
            gmState->makeBid(bidRec);
        } else {
            std::cout << "What does the player in position " << gmState->getNextToAct() + 1 << " bid?" << std::endl;
            std::cin >> plyrBid; // TODO: Input validation
            gmState->makeBid(plyrBid);
        }
    }

    while (gmState->getNextToAct() != -1){
        if (gmState->getNextToAct() == gmState->getHeroPosition()){
            DecisionPoint * dPoint = new DecisionPoint(gmState);
            Card * cardToPlay = dPoint->recommendPlay();
            std::cout << std::endl;
            std::cout << "Recommended play for hero: " << cardToPlay->getCardStr() << std::endl;
            std::cout << std::endl;
            //delete cardToPlay; // I think the return from recommendPlay is a new card, not a pointer to one in gmState?
            for (int i = 0; i < gmState->getCardsRemaining(); i++) {
                if (*(gmState->getCardFromPlyrHands(gmState->getHeroPosition(), i)) == *cardToPlay) {
                    gmState->playCard(i);
                    break;
                }
            }

        } else {
            // Get user input for what player plays & update gameState
            std::cout << "What does the player in position " << gmState->getNextToAct() + 1 << " play?" << std::endl;
            std::cin >> cardPlayed; // TODO: Input validation
            gmState->addCardToPlyrHand(gmState->getNextToAct(), cardPlayed);
            gmState->playCard(0);
        }
    }

    printResults();
}

void Game::printResults(){
    std::cout << std::endl;
    std::cout << "GAME RESULTS" << std::endl;
    std::cout << "------------" << std::endl;
    gmState->calcFinalScores();
    for (int i = 0; i < gmState->getNumPlyrs(); i++) {
        std::cout << "Player " << i + 1;
        if (i == gmState->getHeroPosition()){
            std::cout << " (Hero): ";
        } else {
            std::cout << ": ";
        }
        std::cout << gmState->getFinalScore(i) << std::endl;
    }
    std::cout << std::endl;
}