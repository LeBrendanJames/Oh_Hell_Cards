
#ifndef CARDS_GAMESTATETEST_H
#define CARDS_GAMESTATETEST_H

#include <random>
#include <string>
#include <vector>

// helper functions
GameState* makeGenericGameState();
bool checkGameStateEquality(GameState * leftGmSt, GameState * rightGmSt);
	
// COPY CONSTRUCTOR TESTS 
void testCopyConstructor();
void testBidsEquality();
void testRoundLeadEquality();
void testFinalScoresEquality();
void testPlyrHandsEquality();
void testPlydCrdsEquality();
void testFlippedCardEquality();
void testNoSharedPointers();

// ISTRUMP MEMBER FUNCTION TESTS 
void testIsTrump();
void testCorrectTrump();
void testIncorrectTrump();

// MAKEBID MEMBER FUNCTION 
void testMakeBid();
void makeValidBid();
void makeInvalidLowBid();
void makeInvalidHighBid();

// PLAYCARD MEMBER FUNCTION 
void testPlayCard();

// CARDPREVUSED MEMBER FUNCTION 
void testCardPrevUsed();
void cardNotUsed();
void cardInPlydCrds();
void cardInPlyrHands();
void cardIsFlippedCard();

// CALCFINALSCORES MEMBER FUNCTION 
void testCalcFinalScores();
void testCalcBeforeEndGame();
void testCalcAtEndGame();
	
// ALLHANDSGENERATED MEMBER FUNCTION
void testAllHandsGenerated();
void handsGeneratedAndPlayed();
void handsGeneratedNonePlayed();
void handsGeneratedSomePlayed();

// REVERSEPLAY MEMBER FUNCTION 
void testReversePlay();


#endif //CARDS_GAMESTATETEST_H