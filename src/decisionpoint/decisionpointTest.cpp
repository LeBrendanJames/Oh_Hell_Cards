
#include <chrono>
#include "../card/Card.h"
#include "../gamestate/GameState.h"
#include "DecisionPoint.h"

int main(){
    int testsPassed = 0, testsFailed = 0;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    srand(237); // Seeding the same to force determinism in tests, 237 not significant - just needed something

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
    if (dPoint->recommendPlay()->getCardStr() != "Ah"){
        std::cout << "Error with semi-complete game" << std::endl;
        testsFailed++;
    } else {
        testsPassed++;
    }
    delete game;
    delete dPoint;
    delete flippedCard;
    delete heroHand[0];
    delete heroHand[1];
    delete heroHand;



    // Hero cards in different order
    flippedCard = new Card("3h");
    heroHand = new Card*[2];
    heroHand[0] = new Card("2h");
    heroHand[1] = new Card("Ah");
    game = new GameState(4, 1, 2, flippedCard, heroHand);
    game->makeBid(0);
    game->addCardToPlyrHand(0, "Td");
    game->addCardToPlyrHand(0, "8c");
    game->addCardToPlyrHand(2, "Th");
    game->addCardToPlyrHand(2, "Ts");
    game->addCardToPlyrHand(3, "Jh");
    game->addCardToPlyrHand(3, "Ad");
    dPoint = new DecisionPoint(game);
    int bidFirstOrder = dPoint->recommendBid();
    delete game;
    delete dPoint;
    delete flippedCard;
    delete heroHand[0];
    delete heroHand[1];
    delete heroHand;

    flippedCard = new Card("3h");
    heroHand = new Card*[2];
    heroHand[0] = new Card("Ah");
    heroHand[1] = new Card("2h");
    game = new GameState(4, 1, 2, flippedCard, heroHand);
    game->makeBid(0);
    game->addCardToPlyrHand(0, "Td");
    game->addCardToPlyrHand(0, "8c");
    game->addCardToPlyrHand(2, "Th");
    game->addCardToPlyrHand(2, "Ts");
    game->addCardToPlyrHand(3, "Jh");
    game->addCardToPlyrHand(3, "Ad");
    dPoint = new DecisionPoint(game);
    int bidSecondOrder = dPoint->recommendBid();

    if (bidFirstOrder == bidSecondOrder){
        testsPassed++;
    } else {
        std::cout << "Test Failed - Hero cards in different order produced different recommended bids." << std::endl;
        testsFailed++;
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
        testsPassed++;
    } else {
        std::cout << "TEST: DecisionPoint constructor sets scores vector correctly - FAILED" << std::endl;
        testsFailed++;
    }
    if (dPoint->getScore(20) == -1){
        testsPassed++;
    } else {
        std::cout << "TEST: Invalid index passed to getScore is caught and '-1' returned - FAILED" << std::endl;
        testsFailed++;
    }
    delete dPoint;
    dPoint = nullptr;


    dPoint = new DecisionPoint(game);
    if (dPoint->recommendPlay()->getCardStr() == "Ah"){
        testsPassed++;
    } else {
        std::cout << "TEST: DecisionPoint sims - recommendation correct - FAILED" << std::endl;
        testsFailed++;
    }
    delete dPoint;
    dPoint = nullptr;
    delete game;
    game = nullptr;


    game = new GameState(5, 0, 2, flippedCard, heroHand);
    game->setBid(0, 2);
    game->setBid(1, 0);
    game->setBid(2, 0);
    game->setBid(3, 0);
    game->setBid(4, 0);
    dPoint = new DecisionPoint(game);
    if (dPoint->recommendPlay()->getCardStr() == "Ah"){
        testsPassed++;
    } else {
        std::cout << "TEST: 5 player game (Hero cards 2h/Ah) correct recommendation - FAILED" << std::endl;
        testsFailed++;
    }
    delete dPoint;
    dPoint = nullptr;
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
    dPoint = new DecisionPoint(game);
    if (dPoint->recommendPlay()->getCardStr() == "2h"){
        testsPassed++;
    } else {
        std::cout << "TEST: 5 player game (Hero 2nd pos, cards 2h/Ah) correct rec - FAILED" << std::endl;
        testsFailed++;
    }
    delete dPoint;
    dPoint = nullptr;
    delete game;
    game = nullptr;


    game = new GameState(4, 0, 2, flippedCard, heroHand);
    dPoint = new DecisionPoint(game);
    int optimalBid = dPoint->recommendBid();
    if (optimalBid == 2){
        testsPassed++;
    } else {
        std::cout << "TEST: Optimal Bid with Ah/2h (trump Hearts) == 2 - FAILED" << std::endl;
        testsFailed++;
    }
    delete dPoint;
    dPoint = nullptr;
    delete game;
    game = nullptr;



    game = new GameState(3, 1, 2, flippedCard, heroHand);
    game->makeBid(0);
    dPoint = new DecisionPoint(game);
    optimalBid = dPoint->recommendBid();
    if (optimalBid == 2){
        testsPassed++;
    } else {
        std::cout << "TEST: Optimal Bid from 2nd pos with Ah/2h (trump Hearts) == 2 - FAILED" << std::endl;
        testsFailed++;
    }
    delete dPoint;
    dPoint = nullptr;
    delete game;
    game = nullptr;

    game = new GameState(3, 2, 2, flippedCard, heroHand);
    game->makeBid(0);
    game->makeBid(0);
    dPoint = new DecisionPoint(game);
    optimalBid = dPoint->recommendBid();
    if (optimalBid == 2){
        testsPassed++;
    } else {
        std::cout << "TEST: Optimal Bid from 3rd pos with Ah/2h (trump Hearts) == 2 - FAILED" << std::endl;
        testsFailed++;
    }
    delete dPoint;
    dPoint = nullptr;
    delete game;
    game = nullptr;

    std::cout << std::endl;
    std::cout << "SUMMARY:" << std::endl;
    std::cout << "---------------------" << std::endl;
    std::cout << "Tests Passed: " << testsPassed << ", Tests Failed: " << testsFailed << std::endl;

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Total time (seconds): " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << std::endl;



    return 0;
}