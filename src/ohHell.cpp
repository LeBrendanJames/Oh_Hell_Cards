// Program to find optimal bid and play given game parameters  
// https://en.wikipedia.org/wiki/Oh_Hell

#include <iostream>
#include <vector>
#include <string>
#include "game/Game.h"
#include "gamestate/GameState.h"
#include "decisionpoint/DecisionPoint.h"



int main(){
    int choice, numPlyrs, heroPosition, totalCards, inputBid, enteredCount = 0;
    std::string bidsComplete, inputFlippedCard, enteredCard, inputPlay;

    std::cout << "WELCOME TO THE OH HELL CARD GAME SIMULATOR" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    std::cout << "What would you like to do?" << std::endl;
    std::cout << "Enter '1' to get a recommendation on one single decision." << std::endl;
    std::cout << "Enter '2' to simulate a full game (including a game already in progress)." << std::endl;
    std::cin >> choice;

    if (choice == 1){
        std::cout << "How many players are in the game?" << std::endl;
        std::cin >> numPlyrs;

        std::cout << "What position is Hero in (1 - " << numPlyrs << ")?" << std::endl;
        std::cin >> heroPosition;

        std::cout << "How many cards are in the round?" << std::endl;
        std::cin >> totalCards;

        std::cout << "What is the flipped card in the middle representing trump?" << std::endl;
        std::cin >> inputFlippedCard;
        Card * flippedCard = new Card(inputFlippedCard);

        std::cout << "What is hero's hand?" << std::endl;
        std::cout << "After each card entered, press ENTER, then enter the next one." << std::endl;
        std::vector<std::string> enteredHeroHand;
        while (enteredCount < totalCards){
            std::cin >> enteredCard;
            enteredHeroHand.push_back(enteredCard);
            enteredCount++;
        }
        Card ** heroHand = new Card*[enteredHeroHand.size()] {nullptr};
        for (int i = 0; i < enteredHeroHand.size(); i++){
            heroHand[i] = new Card(enteredHeroHand[i]);
        }

        std::cout << "Have you already completed the bidding round (Y/N)?" << std::endl;
        std::cin >> bidsComplete;

        if (std::toupper(bidsComplete[0]) == 'Y'){
            // build gameState
            GameState * state = new GameState(numPlyrs, heroPosition - 1, totalCards, flippedCard, heroHand);

            // Gather bids in front of hero
            std::cout << "We will now collect the bids of all players:" << std::endl;
            for (int i = 0; i < numPlyrs; i++) {
                if (i == heroPosition - 1){
                    std::cout << "What is hero's bid?" << std::endl;
                } else {
                    std::cout << "What is the bid of the player in position " << i + 1 << "?" << std::endl;
                }
                std::cin >> inputBid;
                state->makeBid(inputBid);
            }

            if (heroPosition == 1){
                DecisionPoint dPoint(state);
                Card * playRec = dPoint.makePlay();

                std::cout << "Play recommendation: " << playRec->getCardStr() << std::endl;
            } else {
                // TODO: this is assuming we're in 1st round of play. Need to generalize to if we've already played >1 round
                std::cout << "We will now collect the plays that have been made in front of hero:" << std::endl;
                for (int i = 0; i < heroPosition - 1; i++){
                    std::cout << "What did the player in position " << i + 1 << " play?" << std::endl;
                    std::cin >> inputPlay;
                    state->addCardToPlyrHand(i, inputPlay);
                    state->playCard(0);
                }

                DecisionPoint dPoint(state);
                Card * playRec = dPoint.makePlay();

                std::cout << "Play recommendation: " << playRec->getCardStr() << std::endl;

                // memory cleanup
                delete state;
            }

        } else if (heroPosition != 1) {
            // build gameState
            GameState * state = new GameState(numPlyrs, heroPosition - 1, totalCards, flippedCard, heroHand);

            // Gather bids in front of hero
            std::cout << "We will now collect the bids of players in front of hero:" << std::endl;
            for (int i = 0; i < heroPosition - 1; i++) {
                std::cout << "What is the bid of the player in position " << i + 1 << "?" << std::endl;
                std::cin >> inputBid;
                state->makeBid(inputBid);
            }

            // Pass into decisionPoint to get a recommended bid
            DecisionPoint dPoint(state);
            int bidRec = dPoint.makeBid();

            std::cout << "Bid recommendation: " << bidRec << std::endl;

            // clean up allocated memory
            delete state;
            delete flippedCard;
            for (int i = 0; i < enteredHeroHand.size(); i++){
                delete heroHand[i];
            }
            delete heroHand;

        } else {
            // build gameState
            GameState * state = new GameState(numPlyrs, heroPosition - 1, totalCards, flippedCard, heroHand);

            // Pass into decisionPoint to get a recommended bid
            DecisionPoint dPoint(state);
            int bidRec = dPoint.makeBid();

            std::cout << "Bid recommendation: " << bidRec << std::endl;
        }

    } else if (choice == 2){
        std::cout << "Choice 2 currently under construction. Check back later." << std::endl;
        std::cout << std::endl;
    }
	
	return 0;
}