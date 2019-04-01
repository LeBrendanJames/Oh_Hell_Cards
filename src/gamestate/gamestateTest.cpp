
#include "gameState.h"
#include "../card/Card.h"

int main(){
    int testsPassed = 0, testsFailed = 0;

    // GAMESTATE CLASS TESTS
    std::cout << std::endl;
    std::cout << "GAMESTATE CLASS TESTS" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    Card * flippedCard = new Card("3h");
    Card ** heroHand = new Card*[2];
    heroHand[0] = new Card("2h");
    heroHand[1] = new Card("Ah");
    GameState * game = new GameState(4, 0, 2, flippedCard, heroHand);
    if (game->getNumPlyrs() == 4){
        //std::cout << "TEST: GameState Constructor Set numPlayers - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: GameState Constructor Set numPlayers - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getTotalCards() == 2){
        //std::cout << "TEST: GameState Constructor Set totalCards - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: GameState Constructor Set totalCards - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getCardsRemaining() == 2){
        //std::cout << "TEST: GameState Constructor Set cardsRemaining - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: GameState Constructor Set cardsRemaining - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getBid(0) == -1){
        //std::cout << "TEST: GameState Constructor Set Default Bids to -1 - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: GameState Constructor Set Default Bids to -1 - FAILED" << std::endl;
        std::cout << "Actual result: " << game->getBid(0) << std::endl;
        testsFailed++;
    }
    game->setBid(0, 1);
    if (game->getBid(0) == 1){
        //std::cout << "TEST: setBid - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: setBid - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getTrump() == Hearts){
        //std::cout << "TEST: getTrump (correct) - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: getTrump (correct) - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getTrump() != Diamonds){
        //std::cout << "TEST: getTrump (incorrect) - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: getTrump (incorrect) - FAILED" << std::endl;
        testsFailed++;
    }
    Card * card = new Card("Th");
    if (game->isTrump(card)){
        //std::cout << "TEST: isTrump (correct) - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: isTrump (correct) - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getNextToAct() == 0){
        testsPassed++;
    } else {
        std::cout << "TEST: getNextToAct() == 0 - FAILED" << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;
    card = new Card("Ts");
    if (!game->isTrump(card)){
        //std::cout << "TEST: isTrump (incorrect) - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: isTrump (incorrect) - FAILED" << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;
    delete game;
    game = nullptr;
    game = new GameState(4, 1, 2, flippedCard, heroHand);
    if (game->getCardFromPlyrHands(1, 0)->getCardStr() == "2h"){
        //std::cout << "TEST: hero in position 1 - Hero hand in correct place - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: hero in position 1 - Hero hand in correct place - FAILED" << std::endl;
        testsFailed++;
    }
    delete game;
    game = nullptr;

    // TEST COPY CONSTRUCTOR
    game = new GameState(4, 0, 2, flippedCard, heroHand);
    GameState * copiedGame = new GameState(*game);
    if (game->getNumPlyrs() == copiedGame->getNumPlyrs()){
        //std::cout << "TEST: Copy constructor numPlyrs - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor numPlyrs - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getHeroPosition() == copiedGame->getHeroPosition()){
        //std::cout << "TEST: Copy constructor heroPosition - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor heroPosition - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getTotalCards() == copiedGame->getTotalCards()){
        //std::cout << "TEST: Copy constructor totalCards - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor totalCards - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getCardsRemaining() == copiedGame->getCardsRemaining()){
        //std::cout << "TEST: Copy constructor cardsRemaining - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor cardsRemaining - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getTrump() == copiedGame->getTrump()){
        testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor trump - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getNextToAct() == copiedGame->getNextToAct()){
        //std::cout << "TEST: Copy constructor nextToAct - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor nextToAct - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getCurrRound() == copiedGame->getCurrRound()){
        //std::cout << "TEST: Copy constructor currRound - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor currRound - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getBid(2) == copiedGame->getBid(2)){
        //std::cout << "TEST: Copy constructor getBid(2) - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor getBid(2) - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getRoundLead(0) == copiedGame->getRoundLead(0)){
        //std::cout << "TEST: Copy constructor getRoundLead(0) - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor getRoundLead(0) - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getFinalScore(1) == copiedGame->getFinalScore(1)){
        testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor finalScores(1) - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getCardFromPlyrHands(0, 0)->getCardStr() == copiedGame->getCardFromPlyrHands(0, 0)->getCardStr()){
        //std::cout << "TEST: Copy constructor plyrHands(0, 0) - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor plyrHands(0, 0) - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getCardFromPlydCrds(0, 0) == copiedGame->getCardFromPlydCrds(0, 0) &&
            game->getCardFromPlydCrds(game->getTotalCards() - 1, game->getNumPlyrs() - 1) ==
            copiedGame->getCardFromPlydCrds(copiedGame->getTotalCards() - 1, copiedGame->getNumPlyrs() - 1)){
        testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor plydCrds(0, 0) && plydCrds(totalCards, numPlyrs)" << std::endl;
        testsFailed++;
    }
    if (*(game->getFlippedCard()) == *(copiedGame->getFlippedCard())){
        testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor flippedCard - FAILED" << std::endl;
        std::cout << "game.flippedCard = " << game->getFlippedCard()->getCardStr() << std::endl;
        std::cout << "copiedgame.flippedCard = " << copiedGame->getFlippedCard()->getCardStr() << std::endl;
        testsFailed++;
    }
    delete game;
    game = nullptr;
    delete copiedGame;
    copiedGame = nullptr;

    //std::cout << std::endl;
    //std::cout << "Testing playCard:" << std::endl;
    game = new GameState(4, 0, 2, flippedCard, heroHand);
    game->addCardToPlyrHand(1, "As");
    game->addCardToPlyrHand(1, "2d");
    game->addCardToPlyrHand(2, "Ks");
    game->addCardToPlyrHand(2, "Jc");
    game->addCardToPlyrHand(3, "Jh");
    game->addCardToPlyrHand(3, "Td");
    if (game->playCard(-1) == false){
        //std::cout << "TEST: invalid playCard(-1) = false - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: invalid playCard(-1) = false - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->playCard(2) == false){
        //std::cout << "TEST: invalid playCard(2) = false - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: invalid playCard(2) = false - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->playCard(0) == true){
        //std::cout << "TEST: playCard('2h') return = true - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: playCard('2h') return = true - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getCardFromPlydCrds(0, 0)->getCardStr() == "2h"){
        //std::cout << "TEST: playCard(0) correctly adds card to plydCrds - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: playCard(0) correctly adds card to plydCrds - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getCardFromPlyrHands(0, 0)->getCardStr() == "Ah"){
        //std::cout << "TEST: playCard(0) correctly moves other card into used position - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: playCard(0) correctly moves other card into used position - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getCardFromPlyrHands(0, 1) == nullptr){
        //std::cout << "TEST: playCard(0) correctly moves remaining cards forward in array - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: playCard(0) correctly moves remaining cards forward in array - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getNextToAct() == 1){
        //std::cout << "TEST: playCard('2h') - getNextToAct - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: playCard('2h') - getNextToAct - FAILED" << std::endl;
        testsFailed++;
    }
    game->playCard(0);
    game->playCard(0);
    game->playCard(0);
    if (game->getCardsRemaining() == 1){
        //std::cout << "TEST: playCard correctly updates cards remaining at end of trick - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: playCard correctly updates cards remaining at end of trick - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->calcFinalScores() == false){
        //std::cout << "TEST: calcFinalScores correctly recognizes game is not finished - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: calcFinalScores correctly recognizes game is not finished - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getNextToAct() == 3){
        //std::cout << "TEST: updateNextToAct correctly finds trick winner - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: updateNextToAct correctly finds trick winner - FAILED" << std::endl;
        testsFailed++;
    }
    game->playCard(0);
    if (game->getNextToAct() == 0){
        //std::cout << "TEST: updateNextToAct correctly wraps - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: updateNextToAct correctly wraps - FAILED" << std::endl;
        testsFailed++;
    }
    game->playCard(0);
    game->playCard(0);
    game->playCard(0);
    if (game->getNextToAct() == -1){
        //std::cout << "TEST: updateNextToAct correctly recognizes end of game - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: updateNextToAct correctly recognizes end of game - FAILED" << std::endl;
        testsFailed++;
    }

    //std::cout << std::endl;
    //std::cout << "Testing calcFinalScores():" << std::endl;
    // Bids must be in so that scores can be calculated
    game->setBid(0, 2);
    game->setBid(1, 0);
    game->setBid(2, 0);
    game->setBid(3, 0);
    game->calcFinalScores();
    if (game->getFinalScore(0) == 1){
        //std::cout << "TEST: calculated player 0 final score correctly - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: calculated player 0 final score correctly - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getFinalScore(1) == 10){
        //std::cout << "TEST: calculated player 1 final score correctly - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: calculated player 1 final score correctly - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getFinalScore(2) == 10){
        //std::cout << "TEST: calculated player 2 final score correctly - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: calculated player 2 final score correctly - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getFinalScore(3) == 1){
        //std::cout << "TEST: calculated player 3 final score correctly - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: calculated player 3 final score correctly - FAILED" << std::endl;
        testsFailed++;
    }
    delete game;
    game = nullptr;

    std::cout << std::endl;
    std::cout << "SUMMARY" << std::endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "Tests Passed: " << testsPassed << ", Tests Failed: " << testsFailed << std::endl;


    return 0;
}