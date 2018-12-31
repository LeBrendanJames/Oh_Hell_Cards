// Program to find optimal bid and play given game parameters  
// https://en.wikipedia.org/wiki/Oh_Hell

#include <iostream>
#include <vector>
#include "gameState.h"
#include "Player.h"

/*

void getGameStateInput(){

}

int findWinner(Card* cards){
	int winnerPos = -1;

	return winnerPos;
}


// recursive playGame function
// takes a current game situation and chooses the max expected points of all the options of cards to play, which is the recursive call 
int playGame(int bid, int totalCards, int position, int totalPlayers, std::vector<int> remainingCards){
	int expectedPoints = 0;
	
	if (totalCards > 1){
		
	} else {
		if (position == 1){
			// if player holds trump
			
			// else
			
		} else {
			// if player holds trump
			
			// else
			
		}
		return expectedPoints;
	}
}


int main(){
	// Number of cards (at start of round, even if you're midway through it)

 	// Number of players

 	// Hero position

 	case BID:
 		// If not first position, get previous bids

 		// get player hand

 		// build player, gameState

 		// Print bid recommendation
 		std::cout << "Bid recommendation: " << std::to_string(player->makeBid()) << std::endl;
 	case PLAY:
 		// Get everyones bids

 		// Get cards played so far (ask how many tricks completed so far and get those,
 		// then get cards played this trick before hero, if hero isn't 1st position)

 		// Make player, gameState

 		// print play recommendation
 		std::cout << "Play recommendation: " << player->makePlay() << std::endl;

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



	return 1;
}


