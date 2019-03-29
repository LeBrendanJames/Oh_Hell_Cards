#include "Card.h"
#include <iostream>
#include <string>

int main() {
    int testsPassed = 0;
    int testsFailed = 0;

    // CARD CLASS TESTS
    std::cout << std::endl;
    std::cout << "CARD CLASS TESTS" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << std::endl;

  // CARD CREATION WITH STRING
    //std::cout << "Testing card creation - 'Th':" << std::endl;
    Card *card = new Card("th");
    if (card->getVal() == 10) {
        //std::cout << "TEST: Set Card Val From String - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Set Card Val From String - FAILED" << std::endl;
        testsFailed++;
    }
    if (card->getSuit() == 2) {
        //std::cout << "TEST: Set Card Suit From String - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Set Card Suit From String - FAILED" << std::endl;
        testsFailed++;
    }
    if (card->getCardStr() == "Th") {
        //std::cout << "TEST: getCardStr() == 'Th' - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: getCardStr() == 'Th' - FAILED" << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;

  // CARD CREATION WITH STRING
    //std::cout << std::endl;
    //std::cout << "Testing card creation - '3s':" << std::endl;
    card = new Card("3s");
    if (card->getVal() == 3) {
        //std::cout << "TEST: Set Card Val From String - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Set Card Val From String - FAILED" << std::endl;
        testsFailed++;
    }
    if (card->getSuit() == 3) {
        //std::cout << "TEST: Set Card Suit From String - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Set Card Suit From String - FAILED" << std::endl;
        testsFailed++;
    }
    if (card->getCardStr() == "3s") {
        //std::cout << "TEST: getCardStr() == '3s' - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: getCardStr() == '3s' - FAILED" << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;

  // CARD CREATION WITH INT AND ENUM
    //std::cout << std::endl;
    //std::cout << "Testing '1, Hearts' card creation:" << std::endl;
    card = new Card(1, Hearts);
    if (card->getVal() == 1) {
        //std::cout << "TEST: Set Card Val From Int - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Set Card Val From Int - FAILED" << std::endl;
        testsFailed++;
    }
    if (card->getSuit() == 2) {
        //std::cout << "TEST: Set Card Suit From ENUM - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Set Card Suit From ENUM - FAILED" << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;

  // CARD CREATION WITH TWO INTEGERS
    //std::cout << std::endl;
    //std::cout << "Testing '6, 2' (6d) card creation:" << std::endl;
    card = new Card(6, 2);
    if (card->getVal() == 6) {
        //std::cout << "TEST: Set Card Val From Int - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Set Card Val From Int - FAILED" << std::endl;
        testsFailed++;
    }
    if (card->getSuit() == Diamonds) {
        //std::cout << "TEST: Set Card Suit From Int - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Set Card Suit From Int - FAILED" << std::endl;
        std::cout << "Actual return: " << card->getSuit() << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;

  // TEST COPY CONSTRUCTOR
    //std::cout << std::endl;
    //std::cout << "Testing Card Copy Constructor:" << std::endl;
    card = new Card("Ah");
    Card *copiedCard = new Card(*card);
    if (card->getCardStr() == copiedCard->getCardStr()) {
        //std::cout << "TEST: Copy Card - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Copy Card - FAILED" << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;
    delete copiedCard;
    copiedCard = nullptr;

  // CARD COMPARISON "=="
    //std::cout << std::endl;
    //std::cout << "Testing comparison operators:" << std::endl;
    card = new Card("Th");
    Card * otherCard = new Card("Th");
    if (*card == *otherCard){
        //std::cout << "TEST: Card comparison '==' - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Card comparison '==' - FAILED" << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;
    delete otherCard;
    otherCard = nullptr;

  // CARD COMPARISON "=="
    card = new Card("Th");
    otherCard = new Card("6d");
    if (!(*card == *otherCard)){
        //std::cout << "TEST: Card comparison '==' on non-equal cards fails correctly - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Card comparison '==' on non-equal cards fails correctly - FAILED" << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;
    delete otherCard;
    otherCard = nullptr;

  // CARD COMPARISON ">"
    card = new Card("Ah");
    otherCard = new Card("9h");
    if (*card > *otherCard){
        //std::cout << "TEST: Card comparison '>' - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Card comparison '>' - FAILED" << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;
    delete otherCard;
    otherCard = nullptr;

  // CARD COMPARISON ">"
    card = new Card("Th");
    otherCard = new Card("Th");
    if (!(*card > *otherCard)){
        //std::cout << "TEST: Card comparison '>' on equal cards fails correctly - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Card comparison '>' on equal cards fails correctly - FAILED" << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;
    delete otherCard;
    otherCard = nullptr;

  // CARD COMPARISON ">"
    card = new Card("Th");
    otherCard = new Card("Jh");
    if (!(*card > *otherCard)){
        //std::cout << "TEST: Card comparison '>' on lower card fails correctly - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Card comparison '>' on lower card fails correctly - FAILED" << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;
    delete otherCard;
    otherCard = nullptr;

  // CARD COMPARISON "<"
    card = new Card("9h");
    otherCard = new Card("Th");
    if (*card < *otherCard){
        //std::cout << "TEST: Card comparison '<' - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Card comparison '<' - FAILED" << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;
    delete otherCard;
    otherCard = nullptr;

  // CARD COMPARISON "<"
    card = new Card("Th");
    otherCard = new Card("Th");
    if (!(*card < *otherCard)){
        //std::cout << "TEST: Card comparison '<' on equal cards fails correctly - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Card comparison '<' on equal cards fails correctly - FAILED" << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;
    delete otherCard;
    otherCard = nullptr;

  // CARD COMPARISON "<"
    card = new Card("Qh");
    otherCard = new Card("Jh");
    if (!(*card < *otherCard)){
        //std::cout << "TEST: Card comparison '<' on higher card fails correctly - PASSED" << std::endl;
        testsPassed++;
    } else {
        std::cout << "TEST: Card comparison '<' on higher card fails correctly - FAILED" << std::endl;
        testsFailed++;
    }
    delete card;
    card = nullptr;
    delete otherCard;
    otherCard = nullptr;


    //std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "CARD TEST SUMMARY" << std::endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "Tests Passed: " << testsPassed << ", Tests Failed: " << testsFailed << std::endl;
    return 0;
}