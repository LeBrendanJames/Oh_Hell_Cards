// Program to find optimal bid and play given game parameters  
// https://en.wikipedia.org/wiki/Oh_Hell

#include <iostream>
#include <vector>
#include "gameState.h"
#include "Player.h"

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




// TESTS
int main(){
	// CARD CLASS TESTS
	std::cout << std::endl;
	std::cout << "CARD CLASS TESTS" << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	Card * card = new Card("Th");
	if (card->getVal() == 10){
		std::cout << "TEST: Set Card Val From String - PASSED" << std::endl;
	} else {
		std::cout << "TEST: Set Card Val From String - FAILED" << std::endl;
	}
	if (card->getSuit() == 2){
		std::cout << "TEST: Set Card Suit From String - PASSED" << std::endl;
	} else {
		std::cout << "TEST: Set Card Suit From String - FAILED" << std::endl;
	}
	delete card;
	card = nullptr;

	card = new Card(1, Hearts);
	if (card->getVal() == 1){
		std::cout << "TEST: Set Card Val From Int - PASSED" << std::endl;
	} else {
		std::cout << "TEST: Set Card Val From Int - FAILED" << std::endl;
	}
	if (card->getSuit() == 2){
		std::cout << "TEST: Set Card Suit From ENUM - PASSED" << std::endl;
	} else {
		std::cout << "TEST: Set Card Suit From ENUM - FAILED" << std::endl;
	}
	delete card;
	card = nullptr;



	// GAMESTATE CLASS TESTS
	std::cout << std::endl;
	std::cout << "GAMESTATE CLASS TESTS" << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	GameState * game = new GameState(4, 1, Diamonds);
	if (game->getNumPlyrs() == 4){
		std::cout << "TEST: GameState Constructor Set numPlayers - PASSED" << std::endl;
	} else {
		std::cout << "TEST: GameState Constructor Set numPlayers - FAILED" << std::endl;
	}
	if (game->getTotalCards() == 1){
		std::cout << "TEST: GameState Constructor Set totalCards - PASSED" << std::endl;
	} else {
		std::cout << "TEST: GameState Constructor Set totalCards - FAILED" << std::endl;
	}
	if (game->getCardsRemaining() == 1){
		std::cout << "TEST: GameState Constructor Set cardsRemaining - PASSED" << std::endl;
	} else {
		std::cout << "TEST: GameState Constructor Set cardsRemaining - FAILED" << std::endl;
	}
	game->decCardsRemaining();
	if (game->getCardsRemaining() == 0){
		std::cout << "TEST: decCardsRemaining - PASSED" << std::endl;
	} else {
		std::cout << "TEST: decCardsRemaining - FAILED" << std::endl;
	}
	if (game->getBid(0) == -1){
		std::cout << "TEST: GameState Constructor Set Default Bids to -1 - PASSED" << std::endl;
	} else {
		std::cout << "TEST: GameState Constructor Set Default Bids to -1 - FAILED" << std::endl;
	}
	game->setBid(0, 1);
	if (game->getBid(0) == 1){
		std::cout << "TEST: setBid - PASSED" << std::endl;
	} else {
		std::cout << "TEST: setBid - FAILED" << std::endl;
	}
	if (game->getTrump() == Diamonds){
		std::cout << "TEST: getTrump (correct) - PASSED" << std::endl;
	} else {
		std::cout << "TEST: getTrump (correct) - FAILED" << std::endl;
	}
	if (game->getTrump() != Hearts){
		std::cout << "TEST: getTrump (incorrect) - PASSED" << std::endl;
	} else {
		std::cout << "TEST: getTrump (incorrect) - FAILED" << std::endl;
	}
	card = new Card("Td");
	if (game->isTrump(card)){
		std::cout << "TEST: isTrump (correct) - PASSED" << std::endl;
	} else {
		std::cout << "TEST: isTrump (correct) - FAILED" << std::endl;
	}
	delete card;
	card = nullptr;
	card = new Card("Ts");
	if (!game->isTrump(card)){
		std::cout << "TEST: isTrump (incorrect) - PASSED" << std::endl;
	} else {
		std::cout << "TEST: isTrump (incorrect) - FAILED" << std::endl;
	}
	delete card;
	card = nullptr;
	delete game;
	game = nullptr;


	// PLAYER TESTS
	std::cout << std::endl;
	std::cout << "PLAYER CLASS TESTS" << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	Player * plyr = new Player(true, 1);
	if (plyr->isHero()){
		std::cout << "TEST: isHero Set True Correctly - PASSED" << std::endl;
	} else {
		std::cout << "TEST: isHero Set True Correctly - FAILED" << std::endl;
	}
	delete plyr;
	plyr = nullptr;

	plyr = new Player(false, 1);
	if (!plyr->isHero()){
		std::cout << "TEST: isHero Set False Correctly - PASSED" << std::endl;
	} else {
		std::cout << "TEST: isHero Set False Correctly - FAILED" << std::endl;
	}
	game = new GameState(4, 1, Diamonds);
	plyr->setHand(game);
	plyr->printHand(); // QUICK TEST - NEED TO FIX
	delete plyr;
	delete game;


	// GAMEPLAY TESTS
	std::cout << std::endl;
	std::cout << "GAMEPLAY TESTS" << std::endl;
	std::cout << "-------------------------------------" << std::endl;



	return 0;
}


