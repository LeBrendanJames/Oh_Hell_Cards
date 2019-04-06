

#include "Game.h"


Game::Game(int numPlayers, int heroPosition, int totalCards, Card * flippedCard, Card ** heroHand) {
    this->gmState = new GameState(numPlayers, heroPosition, totalCards, flippedCard, heroHand);
}

Game::~Game(){
    delete gmState;
}

void Game::setBid(int position, int bid){
    this->gmState->setBid(position, bid);
}

int Game::getBid(int position){
    return this->gmState->getBid(position);
}

void Game::simRound() {
    std::string cardPlayed;

    while (gmState->getCardsRemaining() > 0){

        if (gmState->getNextToAct() == gmState->getHeroPosition()){
            int optimalCardCount[gmState->getCardsRemaining()] {0};

            for (int i = 0; i < NUM_SIMS; i++) {
                DecisionPoint *dPoint = new DecisionPoint(gmState);
                Card * playRec = dPoint->makePlay();

                int j = 0;
                while (playRec->getCardStr() != gmState->getCardFromPlyrHands(gmState->getNextToAct(), j)->getCardStr()){
                    j++;
                }
                optimalCardCount[j]++;

                delete dPoint;
                dPoint = nullptr;
            }

            int maxOptimal = 0;
            for (int i = 1; i < gmState->getCardsRemaining(); i++){
                if (optimalCardCount[i] > maxOptimal){
                    maxOptimal = i;
                }
            }

            std::cout << "Recommended Play: " << gmState->getCardFromPlyrHands(gmState->getNextToAct(), maxOptimal)->getCardStr();
            std::cout << std::endl;

            // Actually make the play
            gmState->playCard(maxOptimal);

        } else {
            // Get user input for what player plays & update gameState
            std::cout << "What does the player in position " << gmState->getNextToAct() + 1 << " play?" << std::endl;
            std::cin >> cardPlayed; // TODO: Input validation

            gmState->addCardToPlyrHand(gmState->getNextToAct(), cardPlayed);
            gmState->playCard(0);
        }

    }
}

void Game::printResults(){
    std::cout << std::endl;
    std::cout << "GAME RESULTS" << std::endl;
    std::cout << "------------" << std::endl;
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