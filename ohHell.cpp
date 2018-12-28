// Program to find optimal bid and play given game parameters  
// https://en.wikipedia.org/wiki/Oh_Hell

#include <iostream>
#include <vector>
#include "gameState.h"

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

	int bid = 0;
	int maxScore = 0;
	
	int totalCards = 1;
	int position = 1;
	int totalPlayers = 4;

	Suit trump = Hearts;
	std::vector<int> remainingCards = {1, 2, 3, 4};
	int tricksTaken = 0;
	
	int curScore = 0;
	for (int i = 0; i <= totalCards; i++){
		curScore = playGame(i);
		if (curScore > maxScore){
			maxScore = curScore;
			bid = i;
		}
	}


	return bid;
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
		std::cout << "TEST: Set Card Val - PASSED" << std::endl;
	} else {
		std::cout << "TEST: Set Card Val - FAILED" << std::endl;
	}

	if (card->getSuit() == 2){
		std::cout << "TEST: Set Card Suit - PASSED" << std::endl;
	} else {
		std::cout << "TEST: Set Card Suit - FAILED" << std::endl;
	}
	delete card;



	// GAMESTATE CLASS TESTS
	std::cout << std::endl;
	std::cout << "GAMESTATE CLASS TESTS" << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	GameState * newGame = new GameState(4, 1, Diamonds);

	delete newGame;



	return 1;
}


