// Program to find optimal bid and play given game parameters  
// https://en.wikipedia.org/wiki/Oh_Hell

#include <iostream>
#include <vector>
#include <string>
#include "game/Game.h"
#include "gamestate/GameState.h"
#include "decisionpoint/DecisionPoint.h"

// FUNCTION PROTOTYPES
GameState* buildCurrGmStFromUser();
Card** gatherHeroHand(int totalCards);
void collectBidsFromUser(GameState * state, int numPositions);
void collectPlayedCardsFromUser(GameState * state);
// Input gathering and validation
void getIntWithValidation(std::string question, int& inputPlace, int low, int high);
bool validInt(int& inputPlace, int low, int high);
void getCardWithValidation(std::string question, std::string& inputPlace);
bool validCard(std::string& inputPlace);
void getYNCharWithValidation(std::string question, std::string& inputPlace);
bool validYNChar(std::string& inputPlace);



int main(){
    int choice;
    GameState * state = nullptr;
    DecisionPoint * dPoint = nullptr;

    std::cout << std::endl;
    std::cout << "WELCOME TO THE OH HELL CARD GAME SIMULATOR" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    std::cout << "What would you like to do?" << std::endl;
    std::cout << "Enter '1' to get a recommendation on one single decision." << std::endl;
    std::cout << "Enter '2' to simulate a full game (including a game already in progress)." << std::endl;
    getIntWithValidation("", choice, 1, 2);

    if (choice == 1){
        state = buildCurrGmStFromUser();
        if (state->getBid(state->getHeroPosition()) != -1){
            dPoint = new DecisionPoint(state);
            Card * playRec = dPoint->recommendPlay();
            std::cout << "\nPLAY RECOMMENDATION: " << playRec->getCardStr() << std::endl;
        } else {
            dPoint = new DecisionPoint(state);
            int bidRec = dPoint->recommendBid();
            std::cout << "\nBID RECOMMENDATION: " << bidRec << std::endl;
        }
        delete dPoint;
        delete state;
    } else if (choice == 2){
        std::cout << "Choice 2 currently under construction. Check back later." << std::endl;
        std::cout << std::endl;
    }
	
	return 0;
}


GameState* buildCurrGmStFromUser(){
    int choice, numPlyrs, heroPosition, totalCards;
    std::string bidsComplete, inputFlippedCard;

    // Get all game specs (through dealing of cards)
    getIntWithValidation("How many players are in the game?", numPlyrs, 2, 8);
    getIntWithValidation("What position is Hero in (1 - " + std::to_string(numPlyrs) + ")?", heroPosition, 1, numPlyrs);
    getIntWithValidation("How many cards are in the round?", totalCards, 1, 51 % numPlyrs);
    getCardWithValidation("What is the flipped card in the middle representing trump?", inputFlippedCard);
    Card * flippedCard = new Card(inputFlippedCard);
    Card ** heroHand = gatherHeroHand(totalCards);

    GameState * state = new GameState(numPlyrs, heroPosition - 1, totalCards, flippedCard, heroHand);

    getYNCharWithValidation("Have you already completed the bidding round (Y/N)?", bidsComplete);
    if (bidsComplete == "Y"){ // All players have bid
        collectBidsFromUser(state, numPlyrs);
        collectPlayedCardsFromUser(state);
    } else if (heroPosition != 1) { // Hero hasn't bid, but players in front of hero have bid
        collectBidsFromUser(state, heroPosition - 1);
    }

    return state;
}

Card** gatherHeroHand(int totalCards){
    int enteredCount = 0;
    std::string enteredCard;
    std::vector<std::string> enteredHeroHand;

    std::cout << "What is hero's hand?" << std::endl;
    std::cout << "After each card entered, press ENTER, then enter the next one." << std::endl;
    while (enteredCount < totalCards){
        getCardWithValidation("", enteredCard);
        enteredHeroHand.push_back(enteredCard);
        enteredCount++;
    }
    Card ** heroHand = new Card*[enteredHeroHand.size()] {nullptr};
    for (int i = 0; i < enteredHeroHand.size(); i++){
        heroHand[i] = new Card(enteredHeroHand[i]);
    }

    return heroHand;
}

void collectBidsFromUser(GameState * state, int numPositions){
    int inputBid = -1;

    std::cout << "We will now collect player's bids:" << std::endl;
    for (int i = 0; i < numPositions; i++) {
        if (i == state->getHeroPosition()){
            getIntWithValidation("What is hero's bid?", inputBid, 0, state->getTotalCards());
        } else {
            getIntWithValidation("What is the bid of the player in position " + std::to_string(i + 1) + "?",
                                 inputBid, 0, state->getTotalCards());
        }
        state->makeBid(inputBid);
    }
}

void collectPlayedCardsFromUser(GameState * state){
    std::string trickPlayed, inputPlay;

    getYNCharWithValidation("Has a full trick been played yet (Y/N)?", trickPlayed);
    if (trickPlayed == "Y"){
        std::cout << "We'll now collect cards for the tricks played:" << std::endl;
        do {
            for (int i = 0; i < state->getNumPlyrs(); i++) {
                if (state->getNextToAct() != state->getHeroPosition()) {
                    getCardWithValidation("What did the player in position " +
                                          std::to_string(state->getNextToAct() + 1) + " play?", inputPlay);
                    state->addCardToPlyrHand(state->getNextToAct(), inputPlay);
                    state->playCard(0);
                } else {
                    bool match = false;
                    do {
                        getCardWithValidation("What card did hero play?", inputPlay);
                        for (int j = 0; j < state->getCardsRemaining(); j++) {
                            if (state->getCardFromPlyrHands(state->getNextToAct(), j)->getCardStr() == inputPlay) {
                                state->playCard(j);
                                match = true;
                                break;
                            }
                        }
                        if (!match){
                            std::cout << "That card doesn't match one in the player's hand." << std::endl;
                            std::cout << "The cards available are: ";
                            for (int j = 0; j < state->getCardsRemaining(); j++){
                                std::cout << state->getCardFromPlyrHands(state->getHeroPosition(), j)->getCardStr();
                                if (j != state->getCardsRemaining() - 1) {
                                    std::cout << ", ";
                                } else {
                                    std::cout << std::endl;
                                }
                            }
                        }
                    } while (!match);
                }
            }
            getYNCharWithValidation("Has another full trick been played (Y/N)?", trickPlayed);
        } while (trickPlayed == "Y");
    }

    if (state->getNextToAct() != state->getHeroPosition()) {
        std::cout << "We will now collect the plays that have been made in front of hero for this trick:"
                  << std::endl;
        while (state->getNextToAct() != state->getHeroPosition()){
            getCardWithValidation("What did the player in position " + std::to_string(state->getNextToAct()) +
                                  " play?", inputPlay);
            state->addCardToPlyrHand(state->getNextToAct(), inputPlay);
            state->playCard(0);
        }
    }
}


/************************************************************************************************************
 *  INPUT VALIDATION HELPER FUNCTIONS
 ***********************************************************************************************************/

void getIntWithValidation(std::string question, int& inputPlace, int low, int high){
    if (question != "") {
        std::cout << question << std::endl;
    }
    std::cout << ">> ";
    while (!validInt(inputPlace, low, high)) {
        std::cout << "Invalid input. Please enter a number between " << low << " and " << high << "." << std::endl;
        std::cout << ">> ";
    }
}

bool validInt(int& inputPlace, int low, int high){
    int i = 0;
    std::string userInput;

    std::cin >> userInput;
    while (i < userInput.length()) {
        if (!isdigit(userInput[i])) {
            return false;
        } else if (std::stoi(userInput) < low || std::stoi(userInput) > high){
            return false;
        }
        i++;
    }
    inputPlace = std::stoi(userInput);
    return true;
}

void getCardWithValidation(std::string question, std::string& inputPlace){
    if (question != "") {
        std::cout << question << std::endl;
    }
    std::cout << ">> ";
    while (!validCard(inputPlace)){
        std::cout << "Invalid input. Please enter [2-9, T, J, Q, K, A][c, d, h, s]." << std::endl;
        std::cout << ">> ";
    }
}

bool validCard(std::string& inputPlace){
    std::string userInput;
    std::cin >> userInput;
    if ((userInput[0] >= '2' && userInput[0] <= '9') || userInput[0] == 'A' ||
        userInput[0] == 'K' || userInput[0] == 'Q' || userInput[0] == 'J' ||
        userInput[0] == 'T'){
        if (userInput[1] == 'c' || userInput[1] == 'd' || userInput[1] == 'h' ||
            userInput[1] == 's'){
            if (userInput.length() == 2) {
                inputPlace = userInput;
                return true;
            }
        }
    }
    return false;
}

void getYNCharWithValidation(std::string question, std::string& inputPlace){
    std::cout << question << std::endl;
    std::cout << ">> ";
    while (!validYNChar(inputPlace)){
        std::cout << "Invalid choice. Choice must be 'Y' or 'N'." << std::endl;
        std::cout << ">> ";
    }
}

bool validYNChar(std::string& inputPlace){
    std::string userInput;
    std::cin >> userInput;
    if (userInput.length() == 1 && (userInput[0] == 'Y' || userInput[0] == 'N')){
        inputPlace = userInput;
        return true;
    }
    return false;
}