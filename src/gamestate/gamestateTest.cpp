
#include "gameState.h"
#include "gamestateTest.h"
#include "../card/Card.h"


int main(){
    int testsPassed = 0, testsFailed = 0;

    // GAMESTATE CLASS TESTS
    std::cout << std::endl;
    std::cout << "GAMESTATE CLASS TESTS" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
	
	testCopyConstructor();

	testIsTrump();
	testMakeBid();
	testPlayCard();
	testCardPrevUsed();
	testCalcFinalScores();
	testAllHandsGenerated();
	testReversePlay();

    std::cout << std::endl;
    std::cout << "COMPLETED GAMESTATE TESTS" << std::endl;

    return 0;
}

/*
* Generates a GameState object with:
*	4 players
*	hero in position 0
*	2 totalCards, with no cards played (so both cards remaining in player hands)
*	flippedCard = 3h
*	heroHand = 2h, Ah (if heroHand == true)
*/
GameState* makeGenericGameState(){
	GameState * state = nullptr;
	Card * flippedCard = new Card("3h");
	Card ** heroHand = new Card*[2];
	heroHand[0] = new Card("2h");
	heroHand[1] = new Card("Ah");

	state = new GameState(4, 0, 2, flippedCard, heroHand);

	delete heroHand[0];
	delete heroHand[1];
	delete heroHand;
	delete flippedCard;
	
	return state;
}

// Function to check if two passed in gameStates are equal 
bool checkStateEquality(GameState* leftGmSt, GameState * rightGmSt){
	// numPlyrs
    if (leftGmSt->getNumPlyrs() != rightGmSt->getNumPlyrs()){
        std::cout << "numPlyrs matching problem" << std::endl;
        return false;
    }

    // heroPosition
    if (leftGmSt->getHeroPosition() != rightGmSt->getHeroPosition()){
        std::cout << "heroPosition matching problem" << std::endl;
        return false;
    }

    // totalCards
    if (leftGmSt->getTotalCards() != rightGmSt->getTotalCards()){
        std::cout << "totalCards matching problem" << std::endl;
        return false;
    }

    // numCardsRemaining
    if (leftGmSt->getCardsRemaining() != rightGmSt->getCardsRemaining()){
        std::cout << "numCardsRemaining matching problem" << std::endl;
        return false;
    }

    // trump
    if (leftGmSt->getTrump() != rightGmSt->getTrump()){
        std::cout << "trump matching problem" << std::endl;
        return false;
    }

    // nextToAct
    if (leftGmSt->getNextToAct() != rightGmSt->getNextToAct()){
        std::cout << "nextToAct matching problem" << std::endl;
        return false;
    }

    // currRound
    if (leftGmSt->getCurrRound() != rightGmSt->getCurrRound()){
        std::cout << "currRound matching problem" << std::endl;
        return false;
    }

    // bids
    for (int i = 0; i < leftGmSt->getNumPlyrs(); i++){
        if (leftGmSt->getBid(i) != rightGmSt->getBid(i)){
            std::cout << "bid matching problem" << std::endl;
            return false;
        }
    }

    // roundLead
    for (int i = 0; i < leftGmSt->getTotalCards(); i++){
        if (leftGmSt->getRoundLead(i) != rightGmSt->getRoundLead(i)){
            std::cout << "roundLead matching problem" << std::endl;
            return false;
        }
    }

    // finalScores
    for (int i = 0; i < leftGmSt->getNumPlyrs(); i++){
        if (leftGmSt->getFinalScore(i) != rightGmSt->getFinalScore(i)){
            std::cout << "finalScore matching problem" << std::endl;
            return false;
        }
    }

    // plyrHands
    // Check no two cards match in a players hand
    for (int i = 0; i < rightGmSt->getNumPlyrs(); i++) {
        for (int j = 0; j < rightGmSt->getCardsRemaining(); j++) {
            for (int k = j + 1; k < rightGmSt->getCardsRemaining(); k++) {
                if (rightGmSt->getCardFromPlyrHands(i, j) != nullptr) {
                    if (rightGmSt->getCardFromPlyrHands(i, k) != nullptr){
                        if (*(rightGmSt->getCardFromPlyrHands(i, j)) == *(rightGmSt->getCardFromPlyrHands(i, k))){
                            std::cout << "plyrHands duplicates cards" << std::endl;
                            return false;
                        }
                    }
                }
            }
        }
    }
    // check each card in rightGmSt matches a card in leftGmSt for same player
    bool noMatch = true;
    for (int i = 0; i < rightGmSt->getNumPlyrs(); i++) {
        for (int j = 0; j < rightGmSt->getCardsRemaining(); j++) {
            for (int k = 0; k < rightGmSt->getCardsRemaining(); k++) {
                if (rightGmSt->getCardFromPlyrHands(i, k) != nullptr &&
                    leftGmSt->getCardFromPlyrHands(i, j) != nullptr) {
                    if (*(rightGmSt->getCardFromPlyrHands(i, j)) == *(leftGmSt->getCardFromPlyrHands(i, k))) {
                        noMatch = false;
                    }
                }
            }
            if (noMatch){
                std::cout << "plyrHands don't match" << std::endl;
                return false;
            }
        }
    }

    // plydCrds
    for (int i = 0; i < leftGmSt->getTotalCards(); i++){
        for (int j = 0; j < leftGmSt->getNumPlyrs(); j++){
            if (leftGmSt->getCardFromPlydCrds(i, j) == nullptr){
                if (rightGmSt->getCardFromPlydCrds(i, j) != nullptr){
                    std::cout << "plydCrds matching problem" << std::endl;
                    return false;
                }
            } else {
                if (rightGmSt->getCardFromPlydCrds(i, j) != nullptr &&
                    leftGmSt->getCardFromPlydCrds(i, j)->getCardStr() != rightGmSt->getCardFromPlydCrds(i, j)->getCardStr()){
                    std::cout << "plydCrds matching problem" << std::endl;
                    return false;
                }
            }
        }
    }

    // flippedCard
    if (leftGmSt->getFlippedCard()->getCardStr() != rightGmSt->getFlippedCard()->getCardStr()){
        std::cout << "flippedCard matching problem" << std::endl;
        return false;
    }
	
	return true;
}


// Only testing members that are pointers and hence need new memory allocation in copy constructor 
void testCopyConstructor(){
	testBidsEquality();
	testRoundLeadEquality();
	testFinalScoresEquality();
	testPlyrHandsEquality();
	testPlydCrdsEquality();
	testFlippedCardEquality();
	
	// Delete original gameState and make sure everything is still accessible from copied game without any dereferencing of freed pointers
	testNoSharedPointers(); 
}

void testBidsEquality(){
	GameState * preState = makeGenericGameState();
	GameState * postState = new GameState(*preState);
	
	for (int i = 0; i < preState->getNumPlyrs(); i++){
		if (preState->getBid(i) != postState->getBid(i)){
			std::cout << "TEST FAILURE: function = testBidsEquality()" << std::endl;
			std::cout << "Bid for player " << i << " in copied gamestate. Expected: " << preState->getBid(i) << std::endl;
			std::cout << "Bid for player " << i << " in copied gamestate. Actual: " << postState->getBid(i) << std::endl;
			std::cout << std::endl;
		}
	}
	
	delete preState;
	delete postState;
}

void testRoundLeadEquality(){
	GameState * preState = makeGenericGameState();
	GameState * postState = new GameState(*preState);
	
	for (int i = 0; i < preState->getTotalCards(); i++){
		if (preState->getRoundLead(i) != postState->getRoundLead(i)){
			std::cout << "TEST FAILURE: function = testRoundLeadEquality()" << std::endl;
			std::cout << "roundLead for round " << i << " in copied gamestate. Expected: " << preState->getRoundLead(i) << std::endl;
			std::cout << "roundLead for round " << i << " in copied gamestate. Actual: " << postState->getRoundLead(i) << std::endl;
			std::cout << std::endl;
		}
	}
	
	delete preState;
	delete postState;
}

void testFinalScoresEquality(){
	GameState * preState = makeGenericGameState();
	GameState * postState = new GameState(*preState);
	
	for (int i = 0; i < preState->getNumPlyrs(); i++){
		if (preState->getFinalScore(i) != postState->getFinalScore(i)){
			std::cout << "TEST FAILURE: function = testFinalScoresEquality()" << std::endl;
			std::cout << "finalScore for player " << i << " in copied gamestate. Expected: " << preState->getFinalScore(i) << std::endl;
			std::cout << "finalScore for player " << i << " in copied gamestate. Actual: " << postState->getFinalScore(i) << std::endl;
			std::cout << std::endl;
		}
	}
	
	delete preState;
	delete postState;
}

void testPlyrHandsEquality(){
	GameState * preState = makeGenericGameState();
	GameState * postState = new GameState(*preState);
	
	for (int i = 0; i < preState->getNumPlyrs(); i++){
		for (int j = 0; j < preState->getCardsRemaining(); j++){
			if (preState->getCardFromPlyrHands(i, j) == nullptr){
				if (postState->getCardFromPlyrHands(i, j) != nullptr){
					std::cout << "TEST FAILURE: function = testPlyrHandsEquality()" << std::endl;
					std::cout << "Cards at location (" << i << ", " << j << ") not equal." << std::endl;
				}
			} else {
				if (postState->getCardFromPlyrHands(i, j) != nullptr && 
				    preState->getCardFromPlyrHands(i, j)->getCardStr() != postState->getCardFromPlyrHands(i, j)->getCardStr()){
					std::cout << "TEST FAILURE: function = testPlyrHandsEquality()" << std::endl;
					std::cout << "Cards at location (" << i << ", " << j << ") not equal." << std::endl;
				}
			}
		}
	}
	
	delete preState;
	delete postState;
}

void testPlydCrdsEquality(){
	GameState * preState = makeGenericGameState();
	GameState * postState = new GameState(*preState);
	
	for (int i = 0; i < preState->getTotalCards(); i++){
		for (int j = 0; j < preState->getNumPlyrs(); j++){
			if (preState->getCardFromPlydCrds(i, j) == nullptr){
				if (postState->getCardFromPlydCrds(i, j) != nullptr){
					std::cout << "TEST FAILURE: function = testPlydCrdsEquality()" << std::endl;
					std::cout << "Cards at location (" << i << ", " << j << ") not equal." << std::endl;
					std::cout << std::endl;
				}
			} else {
				if (postState->getCardFromPlydCrds(i, j) != nullptr && 
				    preState->getCardFromPlydCrds(i, j)->getCardStr() != postState->getCardFromPlydCrds(i, j)->getCardStr()){
					std::cout << "TEST FAILURE: function = testPlydCrdsEquality()" << std::endl;
					std::cout << "Cards at location (" << i << ", " << j << ") not equal." << std::endl;
					std::cout << std::endl;
				}
			}
		}
	}
	
	delete preState;
	delete postState;
}

void testFlippedCardEquality(){
	GameState * preState = makeGenericGameState();
	GameState * postState = new GameState(*preState);
	
	if (preState->getFlippedCard()->getCardStr() != postState->getFlippedCard()->getCardStr()){
		std::cout << "TEST FAILURE: function = testFlippedCardEquality()" << std::endl;
		std::cout << "flippedCard in copied gamestate. Expected: " << preState->getFlippedCard() << std::endl;
		std::cout << "flippedCard in copied gamestate. Actual: " << postState->getFlippedCard() << std::endl;
		std::cout << std::endl;
	}
	
	delete preState;
	delete postState;
}

// Copies gameState then deletes original. Makes sure copied gameState still works fine.
void testNoSharedPointers(){
	GameState * preState = makeGenericGameState();
	GameState * postState = new GameState(*preState);
	delete preState;
	preState = makeGenericGameState();
	
	// Check bids equality
	for (int i = 0; i < preState->getNumPlyrs(); i++){
		if (preState->getBid(i) != postState->getBid(i)){
			std::cout << "TEST FAILURE: function = testNoSharedPointers()" << std::endl;
			std::cout << "Bid for player " << i << " in copied gamestate. Expected: " << preState->getBid(i) << std::endl;
			std::cout << "Bid for player " << i << " in copied gamestate. Actual: " << postState->getBid(i) << std::endl;
			std::cout << std::endl;
		}
	}
	
	// Check finalScores equality
	for (int i = 0; i < preState->getNumPlyrs(); i++){
		if (preState->getFinalScore(i) != postState->getFinalScore(i)){
			std::cout << "TEST FAILURE: function = testNoSharedPointers()" << std::endl;
			std::cout << "finalScore for player " << i << " in copied gamestate. Expected: " << preState->getFinalScore(i) << std::endl;
			std::cout << "finalScore for player " << i << " in copied gamestate. Actual: " << postState->getFinalScore(i) << std::endl;
			std::cout << std::endl;
		}
	}
	
	// Check plyrHands equality
	for (int i = 0; i < preState->getNumPlyrs(); i++){
		for (int j = 0; j < preState->getCardsRemaining(); j++){
			if (preState->getCardFromPlyrHands(i, j) == nullptr){
				if (postState->getCardFromPlyrHands(i, j) != nullptr){
					std::cout << "TEST FAILURE: function = testNoSharedPointers()" << std::endl;
					std::cout << "Cards at location (" << i << ", " << j << ") not equal." << std::endl;
				}
			} else {
				if (postState->getCardFromPlyrHands(i, j) != nullptr && 
				    preState->getCardFromPlyrHands(i, j)->getCardStr() != postState->getCardFromPlyrHands(i, j)->getCardStr()){
					std::cout << "TEST FAILURE: function = testNoSharedPointers()" << std::endl;
					std::cout << "Cards at location (" << i << ", " << j << ") not equal." << std::endl;
				}
			}
		}
	}
	
	// Check plydCrds equality
	for (int i = 0; i < preState->getTotalCards(); i++){
		for (int j = 0; j < preState->getNumPlyrs(); j++){
			if (preState->getCardFromPlydCrds(i, j) == nullptr){
				if (postState->getCardFromPlydCrds(i, j) != nullptr){
					std::cout << "TEST FAILURE: function = testNoSharedPointers()" << std::endl;
					std::cout << "Cards at location (" << i << ", " << j << ") not equal." << std::endl;
					std::cout << std::endl;
				}
			} else {
				if (postState->getCardFromPlydCrds(i, j) != nullptr && 
				    preState->getCardFromPlydCrds(i, j)->getCardStr() != postState->getCardFromPlydCrds(i, j)->getCardStr()){
					std::cout << "TEST FAILURE: function = testNoSharedPointers()" << std::endl;
					std::cout << "Cards at location (" << i << ", " << j << ") not equal." << std::endl;
					std::cout << std::endl;
				}
			}
		}
	}
	
	// Check flipped card equality
	if (preState->getFlippedCard()->getCardStr() != postState->getFlippedCard()->getCardStr()){
		std::cout << "TEST FAILURE: function = testNoSharedPointers()" << std::endl;
		std::cout << "flippedCard's don't match" << std::endl;
		std::cout << std::endl;
	}
	
	delete preState;
	delete postState;
}


void testIsTrump(){
	testCorrectTrump();
	testIncorrectTrump();
}

void testCorrectTrump(){
	GameState * state = makeGenericGameState();
	Card * card = new Card("Ah");

	if (!state->isTrump(card)){
		std::cout << "TEST FAILURE: function = testCorrectTrump(), suit tested = Hearts" << std::endl;
		std::cout << "GameState confirms trump correctly - FAILED" << std::endl;
		std::cout << "Expected result: true ('Ah' is trump), Actual result: false." << std::endl;
	}

	delete card;
	delete state;
}

void testIncorrectTrump(){
	GameState * state = makeGenericGameState();
	Card * card = new Card("As");
	
	if (state->isTrump(card)){
		std::cout << "TEST FAILURE: function = testIncorrectTrump(), suit tested = Spades" << std::endl;
		std::cout << "GameState returns false from isTrump() correctly - FAILED" << std::endl;
		std::cout << "Expected result: false ('As' is not trump), Actual result: true." << std::endl;
	}

	delete card;
	delete state;
}

void testMakeBid(){
	makeValidBid();
	makeInvalidLowBid();
	makeInvalidHighBid();
}

void makeValidBid(){
	GameState * state = makeGenericGameState();
	
	state->makeBid(2);
	if (state->getBid(0) != 2){
		std::cout << "TEST FAILURE: function = makeValidBid(), bid tested = 2" << std::endl;
		std::cout << "Bid recorded correctly in gameState - FAILED" << std::endl;
		std::cout << "Expected result: getBid(0) == 2, Actual result: getBid(0) == " << state->getBid(0) << std::endl;
	}
	if (state->getNextToAct() != 1){
		std::cout << "TEST FAILURE: function = makeValidBid(), bid tested = 2" << std::endl;
		std::cout << "makeBid function advances nextToAct correctly - FAILED" << std::endl;
		std::cout << "Expected result: getNextToAct() == 1, Actual result: getNextToAct() == ";
		std::cout << state->getNextToAct() << std::endl;
	}
	
	delete state;
}

void makeInvalidLowBid(){
	GameState * state = makeGenericGameState();
	
	state->makeBid(-5);
	if (state->getBid(0) != -1){
		std::cout << "TEST FAILURE: function = makeInvalidLowBid(), bid tested = -5" << std::endl;
		std::cout << "Bid not recorded in gameState - FAILED" << std::endl;
		std::cout << "Expected result: getBid(0) == -1 (default), Actual result: getBid(0) == ";
		std::cout << state->getBid(0) << std::endl;
	}
	if (state->getNextToAct() != 0){
		std::cout << "TEST FAILURE: function = makeInvalidLowBid(), bid tested = -5" << std::endl;
		std::cout << "makeBid function does not advance nextToAct when bid is invalid - FAILED" << std::endl;
		std::cout << "Expected result: getNextToAct() == 0, Actual result: getNextToAct() == ";
		std::cout << state->getNextToAct() << std::endl;
	}
	
	delete state;
}

void makeInvalidHighBid(){
	GameState * state = makeGenericGameState();
	
	state->makeBid(5);
	if (state->getBid(0) != -1){
		std::cout << "TEST FAILURE: function = makeInvalidHighBid(), bid tested = 5" << std::endl;
		std::cout << "Bid not recorded in gameState - FAILED" << std::endl;
		std::cout << "Expected result: getBid(0) == -1 (default), Actual result: getBid(0) == ";
		std::cout << state->getBid(0) << std::endl;
	}
	if (state->getNextToAct() != 0){
		std::cout << "TEST FAILURE: function = makeInvalidHighBid(), bid tested = 5" << std::endl;
		std::cout << "makeBid function does not advance nextToAct when bid is invalid - FAILED" << std::endl;
		std::cout << "Expected result: getNextToAct() == 0, Actual result: getNextToAct() == ";
		std::cout << state->getNextToAct() << std::endl;
	}
	
	delete state;
}

void testPlayCard(){
	// 1. Test that card played ends up in correct slot of plydCrds 
	playedCardPlacementCorrect();

	// 2. Test that once two cards are played they end up in correct spots in plydCrds
	multiplePlayedCardPlacementCorrect();

	// 3. Check that once card is played to end a round, the nextToAct player is updated to the trick winner
	roundWinnerNextToAct();

	// 4. Check that numCardsRemaining updated once a round ends as well
	numCardsRemainingDecCorrect();

	// 5. Check that as cards are played they are removed from player hands and the other cards are moved down the array
	cardRemovalUponPlay();

	// 6. Check that once final card is played that nextToAct is set to -1
	nextToActEndGame();

	// 7. Try various invalid plays (there are two ways for a play to be invalid - try them both)
		// 7a. make sure playCard returns false
		// 7b. make sure nothing else is changed about gameState when it returns false 
	invalidPlayCardOutOfRange(-1);
	invalidPlayCardOutOfRange(2);
	invalidPlayNotFollowSuit();
}

void playedCardPlacementCorrect(){
	GameState * state = makeGenericGameState();
	
	state->playCard(0);
	
	if (state->getCardFromPlydCrds(0, 0) == nullptr || state->getCardFromPlydCrds(0, 0)->getCardStr() != "2h"){
		std::cout << "TEST FAILURE: function = playedCardPlacementCorrect(), card played = 2h" << std::endl;
		std::cout << "First card played placed in plydCrds slot (0, 0) - FAILED" << std::endl;
	}
	
	delete state;
}

void multiplePlayedCardPlacementCorrect(){
	GameState * state = makeGenericGameState();
	state->addCardToPlyrHand(1, "Jd");
	state->addCardToPlyrHand(1, "Qs");
	
	state->playCard(0);
	state->playCard(1);
	
	if (state->getCardFromPlydCrds(0, 0) == nullptr || state->getCardFromPlydCrds(0, 0)->getCardStr() != "2h" ||
	state->getCardFromPlydCrds(0, 1) == nullptr || state->getCardFromPlydCrds(0, 1)->getCardStr() != "Qs"){
		std::cout << "TEST FAILURE: function = playedCardPlacementCorrect(), card played = 2h" << std::endl;
		std::cout << "First card played placed in plydCrds slot (0, 0) - FAILED" << std::endl;
	}
	
	delete state;
}

// TODO: Update function to run with a variety of card combinations to better test that GameState determines round winner correctly 
void roundWinnerNextToAct(){
	GameState * state = makeGenericGameState();
	state->addCardToPlyrHand(1, "Jd");
	state->addCardToPlyrHand(1, "Qs");
	state->addCardToPlyrHand(2, "6c");
	state->addCardToPlyrHand(2, "7c");
	state->addCardToPlyrHand(3, "Ks");
	state->addCardToPlyrHand(3, "Tc");
	
	state->playCard(0);
	state->playCard(0);
	state->playCard(0);
	state->playCard(0);
	
	if (state->getNextToAct() != 0){
		std::cout << "TEST FAILURE: function = roundWinnerNextToAct()" << std::endl;
		std::cout << "Player 0 only player to play trump and should be next to act." << std::endl;
		std::cout << "Actual nextToAct = " << state->getNextToAct() << std::endl;
	}
	
	delete state;
}

void numCardsRemainingDecCorrect(){
	GameState * state = makeGenericGameState();
	state->addCardToPlyrHand(1, "Jd");
	state->addCardToPlyrHand(1, "Qs");
	state->addCardToPlyrHand(2, "6c");
	state->addCardToPlyrHand(2, "7c");
	state->addCardToPlyrHand(3, "Ks");
	state->addCardToPlyrHand(3, "Tc");
	
	state->playCard(0);
	state->playCard(0);
	state->playCard(0);
	state->playCard(0);
	
	if (state->getCardsRemaining() != 1){
		std::cout << "TEST FAILURE: function = numCardsRemainingDecCorrect()" << std::endl;
		std::cout << "Started with two cards and each player has played one, so numCardsRemaining should be 1." << std::endl;
		std::cout << "Actual numCardsRemaining = " << state->getCardsRemaining() << std::endl;
	}
	
	delete state;
}

void cardRemovalUponPlay(){
	GameState * state = makeGenericGameState();
	
	state->playCard(0);
	
	if (!(state->getCardFromPlyrHands(0, 0) != nullptr &&
		  state->getCardFromPlydCrds(0, 0)->getCardStr() != "Ah" &&
		  state->getCardFromPlyrHands(0, 1) == nullptr)){
		std::cout << "TEST FAILURE: function = cardRemovalUponPlay(), card played = 2h" << std::endl;
		std::cout << "Card not correctly removed from player hand." << std::endl;
		std::cout << "Expected card to be removed and remaining cards to be slid down into its place." << std::endl;
		std::cout << "Starting cards: 2h, Ah with 2h being played." << std::endl;
		std::cout << "Expected hand after play: Ah, NULL" << std::endl;
		std::cout << "Actual hand after play: ";
		if (state->getCardFromPlyrHands(0, 0) == nullptr){
			std::cout << "NULL";
		} else {
			std::cout << state->getCardFromPlyrHands(0, 0)->getCardStr();
		}
		std::cout << ", ";
		if (state->getCardFromPlyrHands(0, 1) == nullptr){
			std::cout << "NULL";
		} else {
			std::cout << state->getCardFromPlyrHands(0, 1)->getCardStr();
		}
		std::cout << std::endl;
	}
	
	delete state;
}

void nextToActEndGame(){
	GameState * state = makeGenericGameState();
	state->addCardToPlyrHand(1, "Jd");
	state->addCardToPlyrHand(1, "Qs");
	state->addCardToPlyrHand(2, "6c");
	state->addCardToPlyrHand(2, "7c");
	state->addCardToPlyrHand(3, "Ks");
	state->addCardToPlyrHand(3, "Tc");
	
	state->playCard(0);
	state->playCard(0);
	state->playCard(0);
	state->playCard(0);
	state->playCard(0);
	state->playCard(0);
	state->playCard(0);
	state->playCard(0);
	
	if (state->getNextToAct() != -1){
		std::cout << "TEST FAILURE: function = nextToActEndGame()" << std::endl;
		std::cout << "All cards played, so nextToAct should be -1." << std::endl;
		std::cout << "Actual nextToAct = " << state->getNextToAct() << std::endl;
	}
	
	delete state;
}

void invalidPlayCardOutOfRange(int cardPosition){
	GameState * preState = makeGenericGameState();
	GameState * postState = new GameState(*preState);
	
	bool playCardResult = postState->playCard(cardPosition);
	
	if (playCardResult != false){
		std::cout << "TEST FAILURE: function = invalidPlayCardOutOfRange(), card position played = " << cardPosition << std::endl;
		std::cout << "cardPosition is invalid and should result in 'false' return" << std::endl;
	}
	if (!checkStateEquality(preState, postState)){
		std::cout << "TEST FAILURE: function = invalidPlayCardOutOfRange(), card position played = " << cardPosition << std::endl;
		std::cout << "preState != postState, meaning playCard changed something about state " << std::endl;
		std::cout << "even though play was invalid and should have caused playCard to abort before making any changes to gameState." << std::endl;		
	}
	
	delete preState;
	delete postState;
}

// TODO: Generalize function so that I can test different variations of not following suit (trump/not trump/etc.)
void invalidPlayNotFollowSuit(){
	GameState * postState = makeGenericGameState();
	postState->addCardToPlyrHand(1, "Jh");
	postState->addCardToPlyrHand(1, "Qs");
	postState->addCardToPlyrHand(2, "6c");
	postState->addCardToPlyrHand(2, "7c");
	postState->addCardToPlyrHand(3, "Ks");
	postState->addCardToPlyrHand(3, "Tc");
	
	postState->playCard(0);
	GameState * preState = new GameState(*postState);
	
	bool playCardResult = postState->playCard(1); // Invalid as not following suit
	
	if (playCardResult != false){
		std::cout << "TEST FAILURE: function = invalidPlayNotFollowSuit()" << std::endl;
		std::cout << "card attempted to be played does not follow suit when following suit is possible" << std::endl;
	}
	if (!checkStateEquality(preState, postState)){
		std::cout << "TEST FAILURE: function = invalidPlayNotFollowSuit()" << std::endl;
		std::cout << "preState != postState, meaning playCard changed something about state " << std::endl;
		std::cout << "even though play was invalid and should have caused playCard to abort before making any changes to gameState." << std::endl;		
	}
	
	delete preState;
	delete postState;
}

void testCardPrevUsed(){
	cardNotUsed();
	cardInPlydCrds();
	cardInPlyrHands();
	cardIsFlippedCard();
}

void cardNotUsed(){
	GameState * state = makeGenericGameState();
	
	if (state->cardPrevUsed("2s")){
		std::cout << "TEST FAILURE: function = cardNotUsed(), card = 2s" << std::endl;
		std::cout << "cardPrevUsed function incorrectly returns that the card has been used." << std::endl;
	}
	
	delete state;
}

void cardInPlydCrds(){
	GameState * state = makeGenericGameState();
	state->playCard(0);
	
	if (state->cardPrevUsed("2h") == false){
		std::cout << "TEST FAILURE: function = cardInPlyrHands(), card = 2h" << std::endl;
		std::cout << "cardPrevUsed function incorrectly returns that the card has not been used." << std::endl;
		std::cout << "card is equal to one of the cards played, so it should return that the card has been used." << std::endl;
	}
	
	delete state;
}

void cardInPlyrHands(){
	GameState * state = makeGenericGameState();
	
	if (state->cardPrevUsed("2h") == false){
		std::cout << "TEST FAILURE: function = cardInPlyrHands(), card = 2h" << std::endl;
		std::cout << "cardPrevUsed function incorrectly returns that the card has not been used." << std::endl;
		std::cout << "card is equal to one of the cards in hero's hand, so it should return that the card has been used." << std::endl;
	}
	
	delete state;
}

void cardIsFlippedCard(){
	GameState * state = makeGenericGameState();
	
	if (state->cardPrevUsed("3h") == false){
		std::cout << "TEST FAILURE: function = cardInPlyrHands(), card = 3h" << std::endl;
		std::cout << "cardPrevUsed function incorrectly returns that the card has not been used." << std::endl;
		std::cout << "card is equal to flippedCard, so it should return that the card has been used." << std::endl;
	}
	
	delete state;	
}

void testCalcFinalScores(){
	testCalcBeforeEndGame();
	testCalcAtEndGame();
}

void testCalcBeforeEndGame(){
	GameState * state = makeGenericGameState();
	
	if (state->calcFinalScores() != false){
        std::cout << "TEST FAILURE: function = testCalcBeforeEndGame()" << std::endl;
		std::cout << "calcFinalScores correctly recognizes game is not finished - FAILED" << std::endl;
		std::cout << "Expected return value: false, actual return value: " << state->calcFinalScores() << std::endl;
    }
	
	delete state;
}

void testCalcAtEndGame(){
    Card * flippedCard = new Card("3h");
    Card ** heroHand = new Card*[2];
    heroHand[0] = new Card("2h");
    heroHand[1] = new Card("Ah");
	GameState * state = new GameState(4, 0, 2, flippedCard, heroHand);
    state->addCardToPlyrHand(1, "As");
    state->addCardToPlyrHand(1, "2d");
    state->addCardToPlyrHand(2, "Ks");
    state->addCardToPlyrHand(2, "Jc");
    state->addCardToPlyrHand(3, "Jh");
    state->addCardToPlyrHand(3, "Td");
	
	// Play through all 8 cards (2 each)
	for (int i = 0; i < 8; i++){
		state->playCard(0);
	}

    state->setBid(0, 2);
    state->setBid(1, 0);
    state->setBid(2, 0);
    state->setBid(3, 0);
    state->calcFinalScores();
	
    if (state->getFinalScore(0) != 1){
        std::cout << "TEST FAILURE: function = testCalcAtEndGame()" << std::endl;
		std::cout << "calcFinalScores correctly calculates player 0 score - FAILED" << std::endl;
		std::cout << "Expected score: 1, actual score: " << state->getFinalScore(0) << std::endl;
    }
    if (state->getFinalScore(1) != 10){
        std::cout << "TEST FAILURE: function = testCalcAtEndGame()" << std::endl;
		std::cout << "calcFinalScores correctly calculates player 1 score - FAILED" << std::endl;
		std::cout << "Expected score: 1, actual score: " << state->getFinalScore(1) << std::endl;
    }
    if (state->getFinalScore(2) != 10){
        std::cout << "TEST FAILURE: function = testCalcAtEndGame()" << std::endl;
		std::cout << "calcFinalScores correctly calculates player 2 score - FAILED" << std::endl;
		std::cout << "Expected score: 1, actual score: " << state->getFinalScore(2) << std::endl;
    }
    if (state->getFinalScore(3) != 1){
        std::cout << "TEST FAILURE: function = testCalcAtEndGame()" << std::endl;
		std::cout << "calcFinalScores correctly calculates player 3 score - FAILED" << std::endl;
		std::cout << "Expected score: 1, actual score: " << state->getFinalScore(3) << std::endl;
    }
	
    delete state;
}

void testAllHandsGenerated(){
	handsGeneratedAndPlayed();
	handsGeneratedNonePlayed();
	handsGeneratedSomePlayed();
}

void handsGeneratedAndPlayed(){
    Card * flippedCard = new Card("3h");
    Card ** heroHand = new Card*[2];
    heroHand[0] = new Card("2h");
    heroHand[1] = new Card("Ah");
    GameState * state = new GameState(4, 0, 2, flippedCard, heroHand);
    state->addCardToPlyrHand(1, "As");
    state->addCardToPlyrHand(1, "2d");
    state->addCardToPlyrHand(2, "Ks");
    state->addCardToPlyrHand(2, "Jc");
    state->addCardToPlyrHand(3, "Jh");
    state->addCardToPlyrHand(3, "Td");

    // play all cards
    for (int i = 0; i < 8; i++){
        state->playCard(0);
    }

    if (!state->allHandsGenerated()){
        std::cout << "TEST FAILURE: function = handsGaneratedAndPlayed" << std::endl;
        std::cout << "allHandsGenerated function fails to recognize that all hands were" << std::endl;
        std::cout << "generated once they have all been played." << std::endl;
        std::cout << std::endl;
    }

    delete flippedCard;
    for (int i = 0; i < 2; i++){
        delete heroHand[i];
    }
    delete heroHand;
    delete state;
}

void handsGeneratedNonePlayed(){
    Card * flippedCard = new Card("3h");
    Card ** heroHand = new Card*[2];
    heroHand[0] = new Card("2h");
    heroHand[1] = new Card("Ah");
    GameState * state = new GameState(4, 0, 2, flippedCard, heroHand);
    state->addCardToPlyrHand(1, "As");
    state->addCardToPlyrHand(1, "2d");
    state->addCardToPlyrHand(2, "Ks");
    state->addCardToPlyrHand(2, "Jc");
    state->addCardToPlyrHand(3, "Jh");
    state->addCardToPlyrHand(3, "Td");


    if (!state->allHandsGenerated()){
        std::cout << "TEST FAILURE: function = handsGaneratedAndPlayed" << std::endl;
        std::cout << "allHandsGenerated function fails to recognize that all hands were" << std::endl;
        std::cout << "generated even if none have been played." << std::endl;
        std::cout << std::endl;
    }

    delete flippedCard;
    for (int i = 0; i < 2; i++){
        delete heroHand[i];
    }
    delete heroHand;
    delete state;
}

void handsGeneratedSomePlayed(){
    Card * flippedCard = new Card("3h");
    Card ** heroHand = new Card*[2];
    heroHand[0] = new Card("2h");
    heroHand[1] = new Card("Ah");
    GameState * state = new GameState(4, 0, 2, flippedCard, heroHand);
    state->addCardToPlyrHand(1, "As");
    state->addCardToPlyrHand(1, "2d");
    state->addCardToPlyrHand(2, "Ks");
    state->addCardToPlyrHand(2, "Jc");
    state->addCardToPlyrHand(3, "Jh");
    state->addCardToPlyrHand(3, "Td");

    // play all cards
    for (int i = 0; i < 3; i++){
        state->playCard(0);
    }

    if (!state->allHandsGenerated()){
        std::cout << "TEST FAILURE: function = handsGaneratedAndPlayed" << std::endl;
        std::cout << "allHandsGenerated function fails to recognize that all hands were" << std::endl;
        std::cout << "generated after some have been played." << std::endl;
        std::cout << std::endl;
    }

    delete flippedCard;
    for (int i = 0; i < 2; i++){
        delete heroHand[i];
    }
    delete heroHand;
    delete state;
}

void testReversePlay(){
	// Set up a gameState, copy it.
	// Make gameState play something then reverse it
	// check that gameState matches copied gameState 
	GameState * preState = nullptr;
	GameState * postState = nullptr;
	Card * tempCard = nullptr;
	Card * cardPlayed = nullptr;
	Card * flippedCard = nullptr;
	Card ** heroHand = nullptr;
	int handSize, numPlyrs, numPlays, heroPosition, cardPosition;
	bool validPlay = false;
	std::vector<std::string> playedCards;
	
	int numTests = 100;
	
	srand(237);
	
	for (int i = 0; i < numTests; i++){
		// randomly generate some stuff to go into preState GameState constructor 
		flippedCard = new Card(rand() % 13 + 1, rand() % 4 + 1);
		//std::cout << "Generaed flippedCard: " << flippedCard->getCardStr() << std::endl;
		handSize = rand() % 4 + 1;
		//std::cout << "handSize = " << handSize << std::endl;
        numPlyrs = rand() % 4 + 2;
        //std::cout << "numPlyrs = " << numPlyrs << std::endl;
        heroPosition = rand() % numPlyrs;
        //std::cout << "heroPosition = " << heroPosition << std::endl;
        preState = new GameState(numPlyrs, heroPosition, handSize, flippedCard);
        //std::cout << "Created preState with the above info." << std::endl;

        for (int i = 0; i < numPlyrs; i++) {
            for (int j = 0; j < handSize; j++) {
                tempCard = new Card(rand() % 13 + 1, rand() % 4 + 1);
                while (preState->cardPrevUsed(tempCard->getCardStr())) {
                    delete tempCard;
                    tempCard = new Card(rand() % 13 + 1, rand() % 4 + 1);
                }
                //std::cout << "Adding " << tempCard->getCardStr() << " to hero hand." << std::endl;
                preState->addCardToPlyrHand(i, tempCard->getCardStr());
                delete tempCard;
            }
        }

		
		// Copy gameState
        //std::cout << "Copying gameState to postState." << std::endl;
		postState = new GameState(*preState);
		
		// Take randomly generated number of plays and play them
		numPlays = rand() % (handSize * numPlyrs - 1) + 1;
		//std::cout << "numPlays = " << numPlays << std::endl;
		for (int i = 0; i < numPlays; i++){
		    cardPosition = rand() % postState->getCardsRemaining();
		    //std::cout << "cardPosition = " << cardPosition << std::endl;
		    cardPlayed = new Card(postState->getCardFromPlyrHands(postState->getNextToAct(), cardPosition)->getCardStr());
		    //std::cout << "cardPlayed = "
            validPlay = postState->playCard(cardPosition);
            while (!validPlay){
                delete cardPlayed;
                cardPosition = rand() % postState->getCardsRemaining();
                cardPlayed = new Card(postState->getCardFromPlyrHands(postState->getNextToAct(), cardPosition)->getCardStr());
                validPlay = postState->playCard(cardPosition);
            }
            //std::cout << "Playing card " << cardPlayed->getCardStr() << std::endl;
            playedCards.push_back(cardPlayed->getCardStr());
		}
		// Reverse that same number of plays, popping off card to reverse from playedCards vector
		for (int i = 0; i < numPlays; i++){
			postState->reversePlay(playedCards.back());
			playedCards.pop_back();
		}
		//std::cout << "Reversed out all the plays" << std::endl;
		
		if (!checkStateEquality(preState, postState)){
			std::cout << "TEST FAILURE: function = testReversePlay()" << std::endl;
			std::cout << "preState and postState don't match" << std::endl;
			std::cout << std::endl;
		}
		
		delete preState;
		delete postState;
		delete flippedCard;
	}
}






