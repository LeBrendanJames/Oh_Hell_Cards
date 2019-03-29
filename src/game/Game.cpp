

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
            int optimalCardCount[2] {0};
        //if (gmState->getNextToAct() == gmState->getHeroPosition() && gmState->getCardsRemaining() == 2){
            std::cout << "About to create DecisionPoint" << std::endl;
            for (int i = 0; i < NUM_SIMS; i++) {
                DecisionPoint *dPoint = new DecisionPoint(gmState);
                std::cout << "Created DecisionPoint" << std::endl;
                dPoint->genOpponentHands();
                std::cout << "Generated random opponent hands for dPoint" << std::endl;
                Card playRec = dPoint->makePlay();
                std::cout << "Finished dPoint->makePlay()" << std::endl;
                //Card playRec("Ah");
                if (playRec.getCardStr() == "Ah"){
                    optimalCardCount[0]++;
                } else {
                    optimalCardCount[1]++;
                }

                delete dPoint;
                dPoint = nullptr;
            }

            if (optimalCardCount[0] >= optimalCardCount[1]){
                std::cout << "Recommended Play: Ah" << std::endl;
                Card playRec("Ah");
                gmState->addCardPlayed(playRec.getCardStr());
                gmState->removeCardFromHand(gmState->getHeroPosition(), playRec.getCardStr());
            } else {
                std::cout << "Recommended Play: 2h" << std::endl;
                Card playRec("2h");
                gmState->addCardPlayed(playRec.getCardStr());
                gmState->removeCardFromHand(gmState->getHeroPosition(), playRec.getCardStr());
            }
            //std::cout << "Recommended Play: " << playRec.getCardStr() << std::endl;

            //std::cout << "playRec: " << playRec.getCardStr << std::endl;

            // TODO: Make sure play is made and gmState updated (whether its done above or here)
            //gmState->addCardPlayed(playRec.getCardStr());
            //gmState->removeCardFromHand(gmState->getHeroPosition(), playRec.getCardStr());
            gmState->updateNextToAct();

        /*} else if (gmState->getNextToAct() == gmState->getHeroPosition()){
            Card playRec("Kh");
            std::cout << "Recommended Play: " << playRec.getCardStr() << std::endl;
            // TODO: Make sure play is made and gmState updated (whether its done above or here)
            gmState->addCardPlayed(playRec.getCardStr());
            gmState->removeCardFromHand(gmState->getHeroPosition(), playRec.getCardStr());
            gmState->updateNextToAct();
            */
        } else {
            // Get user input for what player plays & update gameState
            std::cout << "What does the player in position " << gmState->getNextToAct() + 1 << " play?" << std::endl;
            std::cin >> cardPlayed; // TODO: Input validation
            gmState->addCardPlayed(cardPlayed);
            //gmState->removeCardFromHand(gmState->getNextToAct(), cardPlayed); // Shouldn't be anything in opponent hands
            gmState->updateNextToAct(); // TODO: Maybe this should be called from addCardPlayed function above?
        }

    }
}

void Game::printResults(){
    int scores[4] = {12,10,10,10}; // JUST FOR TESTING - Scores will have to come from some gameState function

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