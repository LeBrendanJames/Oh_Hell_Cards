
#include "../card/Card.h"
#include "../gamestate/GameState.h"
#include "DecisionPoint.h"

// Function Prototypes
int runPlaySims(int numSims, GameState * gmSt);
int runBidSims(int numSims, GameState * gmSt);

int main(){
    int testsPassed = 0, testsFailed = 0;

    srand(237); // Seeding the same to force determinism in tests

    // DECISIONPOINT TESTS
    std::cout << std::endl;
    std::cout << "DECISIONPOINT CLASS TESTS" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    // Semi-complete game
    Card * flippedCard = new Card("3h");
    Card ** heroHand = new Card*[2];
    heroHand[0] = new Card("2h");
    heroHand[1] = new Card("Ah");
    GameState * game = new GameState(4, 1, 2, flippedCard, heroHand);
    game->setBid(0, 0);
    game->setBid(1, 2);
    game->setBid(2, 0);
    game->setBid(3, 0);
    game->addCardToPlyrHand(0, "5d");
    game->addCardToPlyrHand(2, "6d");
    game->addCardToPlyrHand(3, "7d");
    game->playCard(0);
    game->playCard(0);
    game->playCard(0);
    game->playCard(0);
    auto * dPoint = new DecisionPoint(game);
    if (dPoint->makePlay()->getCardStr() != "Ah"){
        std::cout << "Error with semi-complete game" << std::endl;
    }
    delete game;
    delete dPoint;
    delete flippedCard;
    delete heroHand[0];
    delete heroHand[1];
    delete heroHand;


    flippedCard = new Card("3h");
    heroHand = new Card*[2];
    heroHand[0] = new Card("2h");
    heroHand[1] = new Card("Ah");
    game = new GameState(4, 0, 2, flippedCard, heroHand);
    game->setBid(0, 2);
    game->setBid(1, 0);
    game->setBid(2, 0);
    game->setBid(3, 0);
    dPoint = new DecisionPoint(game);
    if (dPoint->getScore(0) == -1 && dPoint->getScore(3) == -1){
        std::cout << "TEST: DecisionPoint constructor sets scores vector correctly - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: DecisionPoint constructor sets scores vector correctly - FAILED" << std::endl;
        testsFailed++;
    }
    if (dPoint->getScore(20) == -1){
        std::cout << "TEST: Invalid index passed to getScore is caught and '-1' returned - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Invalid index passed to getScore is caught and '-1' returned - FAILED" << std::endl;
        testsFailed++;
    }
    delete dPoint;
    dPoint = nullptr;
    //delete game;
    //game = nullptr;



    std::cout << std::endl;
    std::cout << "Testing genOpponentHands() and makePlay():" << std::endl;

    //int maxScoreCounter = runPlaySims(4, game);
    int maxScoreCounter = runPlaySims(200, game);
    if (game->getCardFromPlyrHands(0, maxScoreCounter)->getCardStr() == "Ah"){
        std::cout << "TEST: 200 DecisionPoint sims - recommendation correct - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: 200 DecisionPoint sims - recommendation correct - FAILED" << std::endl;
        testsFailed++;
    }

    delete game;
    game = nullptr;
    maxScoreCounter = 0;



    std::cout << std::endl;
    std::cout << "Testing different game sizes/hero cards/bid amounts/hero positions:" << std::endl;
    game = new GameState(5, 0, 2, flippedCard, heroHand);
    game->setBid(0, 2);
    game->setBid(1, 0);
    game->setBid(2, 0);
    game->setBid(3, 0);
    game->setBid(4, 0);
    maxScoreCounter = runPlaySims(50, game);
    if (game->getCardFromPlyrHands(0, maxScoreCounter)->getCardStr() == "Ah"){
        std::cout << "TEST: 5 player game (Hero cards 2h/Ah) correct recommendation - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: 5 player game (Hero cards 2h/Ah) correct recommendation - FAILED" << std::endl;
        testsFailed++;
    }
    delete game;
    game = nullptr;




    game = new GameState(5, 1, 2, flippedCard, heroHand);
    game->makeBid(0);
    game->makeBid(2);
    game->makeBid(0);
    game->makeBid(0);
    game->makeBid(0);
    game->addCardToPlyrHand(0, "Qc");
    game->playCard(0);
    maxScoreCounter = runPlaySims(10, game);
    if (game->getCardFromPlyrHands(1, maxScoreCounter)->getCardStr() == "2h"){
        std::cout << "TEST: 5 player game (Hero 2nd pos, cards 2h/Ah) correct rec - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: 5 player game (Hero 2nd pos, cards 2h/Ah) correct rec - FAILED" << std::endl;
        testsFailed++;
    }
    delete game;
    game = nullptr;




    std::cout << std::endl;
    std::cout << "Testing makeBid() recommendation:" << std::endl;

    game = new GameState(4, 0, 2, flippedCard, heroHand);
    int optimalBid = runBidSims(100, game);
    if (optimalBid == 2){
        std::cout << "TEST: Optimal Bid with Ah/2h (trump Hearts) == 2 - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Optimal Bid with Ah/2h (trump Hearts) == 2 - FAILED" << std::endl;
        testsFailed++;
    }
    //delete dPoint;
    //dPoint = nullptr;
    delete game;
    game = nullptr;



    game = new GameState(3, 1, 2, flippedCard, heroHand);
    game->makeBid(0);
    //std::cout << "First player made bid of 0. Next to act = " << game->getNextToAct() << std::endl;
    //dPoint = new DecisionPoint(game);
    //dPoint->genOpponentHands();
    //optimalBid = 0;
    //optimalBid = dPoint->makeBid();
    optimalBid = runBidSims(50, game);
    if (optimalBid == 2){
        std::cout << "TEST: Optimal Bid from 2nd pos with Ah/2h (trump Hearts) == 2 - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Optimal Bid from 2nd pos with Ah/2h (trump Hearts) == 2 - FAILED" << std::endl;
        testsFailed++;
    }
    //delete dPoint;
    //dPoint = nullptr;
    delete game;
    game = nullptr;

    game = new GameState(3, 2, 2, flippedCard, heroHand);
    game->makeBid(0);
    game->makeBid(0);
    //dPoint = new DecisionPoint(game);
    //dPoint->genOpponentHands();
    //optimalBid = 0;
    //optimalBid = dPoint->makeBid();
    optimalBid = runBidSims(50, game);
    if (optimalBid == 2){
        std::cout << "TEST: Optimal Bid from 3rd pos with Ah/2h (trump Hearts) == 2 - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Optimal Bid from 3rd pos with Ah/2h (trump Hearts) == 2 - FAILED" << std::endl;
        testsFailed++;
    }
    //delete dPoint;
    //dPoint = nullptr;
    delete game;
    game = nullptr;

    std::cout << "SUMMARY:" << std::endl;
    std::cout << "---------------------" << std::endl;
    std::cout << "Tests Passed: " << testsPassed << ", Tests Failed: " << testsFailed << std::endl;


    return 0;
}



int runPlaySims(int numSims, GameState * gmSt){
    //std::cout << "In runPlaySims" << std::endl;
    Card * playRec = nullptr;
    DecisionPoint * dPoint = nullptr;
    int scoreCounter[gmSt->getCardsRemaining()] {0};
    for (int i = 0; i < numSims; i++){
        //std::cout << "In runPlaySims function main loop" << std::endl;
        dPoint = new DecisionPoint(gmSt);
        //dPoint->genOpponentHands();
        //std::cout << "Through genOpponentHands" << std::endl;
        playRec = dPoint->makePlay();
        //std::cout << "Through makePlay. playRec = " << playRec->getCardStr() << std::endl;
        for (int j = 0; j < gmSt->getCardsRemaining(); j++){
            if (*playRec == *(gmSt->getCardFromPlyrHands(gmSt->getHeroPosition(), j))){
                scoreCounter[j]++;
            }
        }

        delete dPoint;
        dPoint = nullptr;
        playRec = nullptr;

        //std::cout << "Through one simulation loop, going back to top" << std::endl;
    }

    std::cout << "Done with sims. Reommendations:" << std::endl;
    std::cout << "2h: " << scoreCounter[0] << std::endl;
    std::cout << "Ah: " << scoreCounter[1] << std::endl;

    int maxScoreCounter = 0;
    for (int i = 1; i < gmSt->getCardsRemaining(); i++){
        if (scoreCounter[i] > scoreCounter[maxScoreCounter]){
            maxScoreCounter = i;
        }
    }

    return maxScoreCounter;
}

int runBidSims(int numSims, GameState * gmSt){
    DecisionPoint * dPoint = nullptr;
    int scoreCounter[gmSt->getCardsRemaining() + 1] {0};
    for (int i = 0; i < numSims; i++){
        //std::cout << "********NEXT SIM************" << std::endl;
        dPoint = new DecisionPoint(gmSt);

        // Add 1 to index array of whatever bid is recommended
        scoreCounter[dPoint->makeBid()]++;

        delete dPoint;
        dPoint = nullptr;
    }

    for (int i = 0; i < gmSt->getCardsRemaining() + 1; i++){
        std::cout << "Bid " << i << ": " << scoreCounter[i] << std::endl;
    }

    int maxScoreCounter = 0;
    for (int i = 1; i <= gmSt->getCardsRemaining(); i++){
        if (scoreCounter[i] > scoreCounter[maxScoreCounter]){
            maxScoreCounter = i;
        }
    }

    return maxScoreCounter;
}