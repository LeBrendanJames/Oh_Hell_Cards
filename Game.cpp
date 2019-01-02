

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
    //std::cout << "In simRound." << std::endl;
    std::string cardPlayed;

    while (gmState->getCardsRemaining() > 0){
        //std::cout << "Cards Remaining: " << gmState->getCardsRemaining() << std::endl;
        //gmState->decCardsRemaining();

        //std::cout << "Next to act (output version): " << gmState->getNextToAct() + 1 << std::endl;

        if (gmState->getNextToAct() == gmState->getHeroPosition() && gmState->getCardsRemaining() == 2){
            //DecisionPoint * dPoint = new DecisionPoint(&gmState);
            //Card playRec = dPoint->makePlay(&gmState);
            Card playRec("Ah");

            std::cout << "Recommended Play: " << playRec.getCardStr() << std::endl;

            // TODO: Make sure play is made and gmState updated (whether its done above or here)
            gmState->addCardPlayed(playRec.getCardStr());
            gmState->removeCardFromHand(gmState->getHeroPosition(), playRec.getCardStr());
            gmState->updateNextToAct();

        } else if (gmState->getNextToAct() == gmState->getHeroPosition()){
            Card playRec("Kh");
            std::cout << "Recommended Play: " << playRec.getCardStr() << std::endl;
            // TODO: Make sure play is made and gmState updated (whether its done above or here)
            gmState->addCardPlayed(playRec.getCardStr());
            gmState->removeCardFromHand(gmState->getHeroPosition(), playRec.getCardStr());
            gmState->updateNextToAct();

        } else {
                // Get user input for what player plays & update gameState
            std::cout << "What does the player in position " << gmState->getNextToAct() + 1 << " play?" << std::endl;
            std::cin >> cardPlayed; // TODO: Input validation
            gmState->addCardPlayed(cardPlayed);
            gmState->updateNextToAct(); // TODO: Maybe this should be called from addCardPlayed function above?
        }

    }
}

void Game::printResults(){
    int scores[4] = {12,0,0,0}; // JUST FOR TESTING - Scores will have to come from some gameState function

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
        std::cout << scores[i] << std::endl;
    }
}