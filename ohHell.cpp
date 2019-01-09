// Program to find optimal bid and play given game parameters  
// https://en.wikipedia.org/wiki/Oh_Hell

#include <iostream>
#include <vector>
//#include "Game.h"
#include "GameState.h"
#include "DecisionPoint.h"

/*
int main(){
	// Get user input on current game
		// How many players?
		// Hero position?
		// Number of cards in round?
		// Any bids yet?
		// Any full tricks played yet?
		// Any cards played yet?
	
	// Create Game & GameState 
	
	game->simRound();
	
	
	return 0;
}
*/


/*
int main(){
    Card * flippedCard = new Card("Th");
    std::cout << "Flipped Card: " << flippedCard->getCardStr() << std::endl;

    Card ** heroHand = new Card*[2];
    heroHand[0] = new Card("2h");
    heroHand[1] = new Card("Ah");
    std::cout << "Hero hand card #1: " << heroHand[0]->getCardStr() << std::endl;
    std::cout << "Hero hand card #2: " << heroHand[1]->getCardStr() << std::endl;

    int numPlayers = 4;
    int heroPosition = 1;
    int totalCards = 2;

    Game * game = new Game(numPlayers, heroPosition, totalCards, flippedCard, heroHand);

    // Set bids
    game->setBid(1, 2);
    game->setBid(2, 0);
    game->setBid(3, 0);
    game->setBid(4, 0);

    for (int i = 0; i < numPlayers; i++){
        std::cout << "Player #" << i + 1 << " bid: " << game->getBid(i + 1) << std::endl;
    }

    game->simRound();

    game->printResults();

    delete game;

    return 0;
}
*/



// TESTS
int main(){

	int testsPassed = 0, testsFailed = 0;

	// CARD CLASS TESTS
	std::cout << std::endl;
	std::cout << "CARD CLASS TESTS" << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	std::cout << std::endl;

	std::cout << "Testing card creation - 'Th'" << std::endl;
	Card * card = new Card("th");
	if (card->getVal() == 10){
		std::cout << "TEST: Set Card Val From String - PASSED" << std::endl;
		testsPassed++;
	} else {
		std::cout << "TEST: Set Card Val From String - FAILED" << std::endl;
		testsFailed++;
	}
	if (card->getSuit() == 2){
		std::cout << "TEST: Set Card Suit From String - PASSED" << std::endl;
		testsPassed++;
	} else {
		std::cout << "TEST: Set Card Suit From String - FAILED" << std::endl;
		testsFailed++;
	}
	if (card->getCardStr() == "Th"){
	    std::cout << "TEST: getCardStr() == 'Th' - PASSED" << std::endl;
		testsPassed++;
	} else {
        std::cout << "TEST: getCardStr() == 'Th' - FAILED" << std::endl;
        testsFailed++;
	}
	delete card;
	card = nullptr;

	std::cout << std::endl;
	std::cout << "Testing card creation - '3s'" << std::endl;
    card = new Card("3s");
    if (card->getVal() == 3){
        std::cout << "TEST: Set Card Val From String - PASSED" << std::endl;
		testsPassed++;
    } else {
        std::cout << "TEST: Set Card Val From String - FAILED" << std::endl;
        testsFailed++;
    }
    if (card->getSuit() == 3){
        std::cout << "TEST: Set Card Suit From String - PASSED" << std::endl;
		testsPassed++;
    } else {
        std::cout << "TEST: Set Card Suit From String - FAILED" << std::endl;
        testsFailed++;
    }
    if (card->getCardStr() == "3s"){
        std::cout << "TEST: getCardStr() == '3s' - PASSED" << std::endl;
		testsPassed++;
    } else {
        std::cout << "TEST: getCardStr() == '3s' - FAILED" << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;

    std::cout << std::endl;
    std::cout << "Testing '1, Hearts' card creation" << std::endl;
	card = new Card(1, Hearts);
	if (card->getVal() == 1){
		std::cout << "TEST: Set Card Val From Int - PASSED" << std::endl;
		testsPassed++;
	} else {
		std::cout << "TEST: Set Card Val From Int - FAILED" << std::endl;
		testsFailed++;
	}
	if (card->getSuit() == 2){
		std::cout << "TEST: Set Card Suit From ENUM - PASSED" << std::endl;
		testsPassed++;
	} else {
		std::cout << "TEST: Set Card Suit From ENUM - FAILED" << std::endl;
		testsFailed++;
	}
	delete card;
	card = nullptr;

	std::cout << std::endl;
    std::cout << "Testing '6, 1' (6d) card creation" << std::endl;
    card = new Card(6, 2);
    if (card->getVal() == 6){
        std::cout << "TEST: Set Card Val From Int - PASSED" << std::endl;
		testsPassed++;
    } else {
        std::cout << "TEST: Set Card Val From Int - FAILED" << std::endl;
        testsFailed++;
    }
    if (card->getSuit() == Diamonds){
        std::cout << "TEST: Set Card Suit From Int - PASSED" << std::endl;
		testsPassed++;
    } else {
        std::cout << "TEST: Set Card Suit From Int - FAILED" << std::endl;
        std::cout << "Actual return: " << card->getSuit() << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;

    // test copy constructor
    std::cout << std::endl;
    std::cout << "Testing Card Copy Constructor" << std::endl;
    card = new Card("Ah");
    Card * copiedCard = new Card(*card);
    if (card->getCardStr() == copiedCard->getCardStr()){
        std::cout << "TEST: Copy Card - PASSED" << std::endl;
		testsPassed++;
    } else {
        std::cout << "TEST: Copy Card - FAILED" << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;
    delete copiedCard;
    copiedCard = nullptr;



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
		std::cout << "TEST: GameState Constructor Set numPlayers - PASSED" << std::endl;
		testsPassed++;
	} else {
		std::cout << "TEST: GameState Constructor Set numPlayers - FAILED" << std::endl;
		testsFailed++;
	}
	if (game->getTotalCards() == 2){
		std::cout << "TEST: GameState Constructor Set totalCards - PASSED" << std::endl;
		testsPassed++;
	} else {
		std::cout << "TEST: GameState Constructor Set totalCards - FAILED" << std::endl;
		testsFailed++;
	}
	if (game->getCardsRemaining() == 2){
		std::cout << "TEST: GameState Constructor Set cardsRemaining - PASSED" << std::endl;
		testsPassed++;
	} else {
		std::cout << "TEST: GameState Constructor Set cardsRemaining - FAILED" << std::endl;
		testsFailed++;
	}
	/*game->decCardsRemaining();
	if (game->getCardsRemaining() == 1){
		std::cout << "TEST: decCardsRemaining - PASSED" << std::endl;
		testsPassed++;
	} else {
		std::cout << "TEST: decCardsRemaining - FAILED" << std::endl;
		testsFailed++;
	}*/
	if (game->getBid(0) == -1){
		std::cout << "TEST: GameState Constructor Set Default Bids to -1 - PASSED" << std::endl;
		testsPassed++;
	} else {
		std::cout << "TEST: GameState Constructor Set Default Bids to -1 - FAILED" << std::endl;
		std::cout << "Actual result: " << game->getBid(0) << std::endl;
		testsFailed++;
	}
	game->setBid(0, 1);
	if (game->getBid(0) == 1){
		std::cout << "TEST: setBid - PASSED" << std::endl;
		testsPassed++;
	} else {
		std::cout << "TEST: setBid - FAILED" << std::endl;
		testsFailed++;
	}
	if (game->getTrump() == Hearts){
		std::cout << "TEST: getTrump (correct) - PASSED" << std::endl;
		testsPassed++;
	} else {
		std::cout << "TEST: getTrump (correct) - FAILED" << std::endl;
		testsFailed++;
	}
	if (game->getTrump() != Diamonds){
		std::cout << "TEST: getTrump (incorrect) - PASSED" << std::endl;
		testsPassed++;
	} else {
		std::cout << "TEST: getTrump (incorrect) - FAILED" << std::endl;
		testsFailed++;
	}
	card = new Card("Th");
	if (game->isTrump(card)){
		std::cout << "TEST: isTrump (correct) - PASSED" << std::endl;
		testsPassed++;
	} else {
		std::cout << "TEST: isTrump (correct) - FAILED" << std::endl;
		testsFailed++;
	}
	delete card;
	card = nullptr;
	card = new Card("Ts");
	if (!game->isTrump(card)){
		std::cout << "TEST: isTrump (incorrect) - PASSED" << std::endl;
		testsPassed++;
	} else {
		std::cout << "TEST: isTrump (incorrect) - FAILED" << std::endl;
		testsFailed++;
	}
	delete card;
	card = nullptr;
	delete game;
	game = nullptr;

	// check copy constructor
    std::cout << std::endl;
    std::cout << "Testing GameState copy constructor:" << std::endl;
    game = new GameState(4, 0, 2, flippedCard, heroHand);
    GameState * copiedGame = new GameState(*game);
    if (game->getNumPlyrs() == copiedGame->getNumPlyrs()){
        std::cout << "TEST: Copy constructor numPlyrs - PASSED" << std::endl;
		testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor numPlyrs - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getHeroPosition() == copiedGame->getHeroPosition()){
        std::cout << "TEST: Copy constructor heroPosition - PASSED" << std::endl;
		testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor heroPosition - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getTotalCards() == copiedGame->getTotalCards()){
        std::cout << "TEST: Copy constructor totalCards - PASSED" << std::endl;
		testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor totalCards - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getCardsRemaining() == copiedGame->getCardsRemaining()){
        std::cout << "TEST: Copy constructor cardsRemaining - PASSED" << std::endl;
		testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor cardsRemaining - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getNextToAct() == copiedGame->getNextToAct()){
        std::cout << "TEST: Copy constructor nextToAct - PASSED" << std::endl;
		testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor nextToAct - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getCurrRound() == copiedGame->getCurrRound()){
        std::cout << "TEST: Copy constructor currRound - PASSED" << std::endl;
		testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor currRound - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getBid(2) == copiedGame->getBid(2)){
        std::cout << "TEST: Copy constructor getBid(2) - PASSED" << std::endl;
		testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor getBid(2) - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getRoundLead(0) == copiedGame->getRoundLead(0)){
        std::cout << "TEST: Copy constructor getRoundLead(0) - PASSED" << std::endl;
		testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor getRoundLead(0) - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getCardFromPlyrHands(0, 0)->getCardStr() == copiedGame->getCardFromPlyrHands(0, 0)->getCardStr()){
        std::cout << "TEST: Copy constructor plyrHands(0, 0) - PASSED" << std::endl;
		testsPassed++;
    } else {
        std::cout << "TEST: Copy constructor plyrHands(0, 0) - FAILED" << std::endl;
        testsFailed++;
    }
    delete game;
    game = nullptr;
    delete copiedGame;
    copiedGame = nullptr;

    std::cout << std::endl;
    std::cout << "Testing playCard:" << std::endl;
    game = new GameState(4, 0, 2, flippedCard, heroHand);
    game->addCardToPlyrHand(1, "As");
    game->addCardToPlyrHand(1, "2d");
    game->addCardToPlyrHand(2, "Ks");
    game->addCardToPlyrHand(2, "Jc");
    game->addCardToPlyrHand(3, "Jh");
    game->addCardToPlyrHand(3, "Td");
    if (game->playCard(-1) == false){
       std::cout << "TEST: invalid playCard(-1) = false - PASSED" << std::endl;
       testsPassed++;
    } else {
        std::cout << "TEST: invalid playCard(-1) = false - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->playCard(2) == false){
        std::cout << "TEST: invalid playCard(2) = false - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: invalid playCard(2) = false - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->playCard(0) == true){
        std::cout << "TEST: playCard('2h') return = true - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: playCard('2h') return = true - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getCardFromPlydCrds(0, 0)->getCardStr() == "2h"){
        std::cout << "TEST: playCard(0) correctly adds card to plydCrds - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: playCard(0) correctly adds card to plydCrds - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getCardFromPlyrHands(0, 0)->getCardStr() == "Ah"){
        std::cout << "TEST: playCard(0) correctly moves other card into used position - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: playCard(0) correctly moves other card into used position - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getCardFromPlyrHands(0, 1) == nullptr){
        std::cout << "TEST: playCard(0) correctly moves remaining cards forward in array - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: playCard(0) correctly moves remaining cards forward in array - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getNextToAct() == 1){
        std::cout << "TEST: playCard('2h') - getNextToAct - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: playCard('2h') - getNextToAct - FAILED" << std::endl;
        testsFailed++;
    }
    game->playCard(0);
    game->playCard(0);
    game->playCard(0);
    if (game->getCardsRemaining() == 1){
        std::cout << "TEST: playCard correctly updates cards remaining at end of trick - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: playCard correctly updates cards remaining at end of trick - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->calcFinalScores() == false){
        std::cout << "TEST: calcFinalScores correctly recognizes game is not finished - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: calcFinalScores correctly recognizes game is not finished - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getNextToAct() == 3){
        std::cout << "TEST: updateNextToAct correctly finds trick winner - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: updateNextToAct correctly finds trick winner - FAILED" << std::endl;
        testsFailed++;
    }
    game->playCard(0);
    if (game->getNextToAct() == 0){
        std::cout << "TEST: updateNextToAct correctly wraps - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: updateNextToAct correctly wraps - FAILED" << std::endl;
        testsFailed++;
    }
    game->playCard(0);
    game->playCard(0);
    game->playCard(0);
    if (game->getNextToAct() == -1){
        std::cout << "TEST: updateNextToAct correctly recognizes end of game - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: updateNextToAct correctly recognizes end of game - FAILED" << std::endl;
        testsFailed++;
    }

    std::cout << std::endl;
    std::cout << "Testing calcFinalScores():" << std::endl;
    // Bids must be in so that scores can be calculated
    game->setBid(0, 2);
    game->setBid(1, 0);
    game->setBid(2, 0);
    game->setBid(3, 0);
    game->calcFinalScores();
    if (game->getFinalScore(0) == 1){
        std::cout << "TEST: calculated player 0 final score correctly - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: calculated player 0 final score correctly - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getFinalScore(1) == 10){
        std::cout << "TEST: calculated player 1 final score correctly - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: calculated player 1 final score correctly - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getFinalScore(2) == 10){
        std::cout << "TEST: calculated player 2 final score correctly - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: calculated player 2 final score correctly - FAILED" << std::endl;
        testsFailed++;
    }
    if (game->getFinalScore(3) == 1){
        std::cout << "TEST: calculated player 3 final score correctly - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: calculated player 3 final score correctly - FAILED" << std::endl;
        testsFailed++;
    }
    delete game;
    game = nullptr;


    // DECISIONPOINT TESTS
    std::cout << std::endl;
    std::cout << "DECISIONPOINT CLASS TESTS" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    game = new GameState(4, 0, 2, flippedCard, heroHand);
    game->setBid(0, 2);
    game->setBid(1, 0);
    game->setBid(2, 0);
    game->setBid(3, 0);
    auto * dPoint = new DecisionPoint(game);
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
    std::cout << std::endl;
    std::cout << "Testing genOpponentHands():" << std::endl;
    dPoint->genOpponentHands();

    Card * playRec = dPoint->makePlay();
    std::cout << "Recommended play: " << playRec->getCardStr() << std::endl;

    delete dPoint;
    delete game;

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "SUMMARY" << std::endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "Tests Passed: " << testsPassed << ", Tests Failed: " << testsFailed << std::endl;
	return 0;
}



