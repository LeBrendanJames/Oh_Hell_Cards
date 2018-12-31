

#include "Player.h"


Player::Player(bool hero, int position) {
    this->hero = hero;
    this->position = position;
}

bool Player::isHero() {
    return hero;
}

int Player::getPosition() {
    return position;
}

void Player::setHand(GameState * currState, std::string * inputCards){
    if(isHero() && inputCards != nullptr){
        for (int i = 0; i < currState->getTotalCards(); i++) {
            plyrHand.push_back(Card(inputCards[i]));
        }
    } else {
        // Random cards
        srand(time(NULL));
        int suitNum = rand() % 4;
        Suit newSuit;
        if (suitNum == 0) {
            newSuit = Clubs;
        } else if (suitNum == 1){
            newSuit = Diamonds;
        } else if (suitNum == 2){
            newSuit = Hearts;
        } else { // 3
            newSuit = Spades;
        }
        plyrHand.push_back(Card(rand() % 13 + 1, newSuit));
        for (int i = 1; i < currState->getTotalCards(); i++){ // For each card
            int newVal = 0;
            bool matchPrevCard = false;
            do {
                newVal = rand() % 13 + 1; // gen value
                suitNum = rand() % 4; // gen suit
                for (int j = i - 1; j >= 0; j++) { // check it isn't duplicate of previous cards
                    if (newVal == plyrHand[j].getVal() && newSuit == plyrHand[j].getSuit()){
                        matchPrevCard = true;
                        break;
                    }
                }
            } while (matchPrevCard);
            if (suitNum == 0) {
                newSuit = Clubs;
            } else if (suitNum == 1){
                newSuit = Diamonds;
            } else if (suitNum == 2){
                newSuit = Hearts;
            } else { // 3
                newSuit = Spades;
            }
            plyrHand.push_back(Card(rand() % 13 + 1, newSuit));
        }
    }
}

void Player::printHand(){
    for (int i = 0; i < plyrHand.size(); i++){
        std::cout << plyrHand[i].getCardStr() << std::endl;
    }
}

// Make optimal bid based on findings from game simulations
int Player::makeBid(GameState * currState) {
    int bidChoice = -1;

    // for number of sims
        // Copy gameState (to have one of the players own to use)
        // Randomize other players cards
        // Run simulation (can make this its own function - return array of expected points by bid amount)
            // for bid from 0 up to max
                // exPts[bidNum] = gameResult after bidding whatever --> Player::makeBid
            // Find max expected points out of all the bids and save that bid as the recommendation

        // Take results of simulation and add to master array of points by bid

    // Simulation
        // Copy gameState
        // if bid = blank then
            // for bid = 0 up to max
                // update gameState for bid
                // exPts[bidNum] = runSimulation(new gameState)
        // else
            // for play = first available card to last available card
                // results += find play (this is the function where the recursion eventually bottoms out)
            // return row of results that's best for whichever player is the one to act here

        // return results

    // Find Play
        //



    return bidChoice;
}


/*
double Player::simDecision(Card * cardPlayed) {
    // Make theoretical gameState (current gameState + card played)
    // create game with all the same attributes except playerVantagePoint changes
    if (!endRound){
        for (int i = 0; i < currState->getCardsRemaining(); i++){
            simDecision();
        }
        // return max of simDecision() above (which i is max = which card to play)
    } else if (endRound && currState->getCardsRemaining() > 1) {
        // play last card

        // figure out who wins trick and update game state
        leadPosition = currState->determineTrickWinner();

        // simGame from that point, passing in who's play it is
        for (int i = 0; i < currState->getCardsRemaining(); i++){
            simDecision();
        }
        // return max of simDecision()
    }
    // base case is last player plays and points are tallied
    // Pass hero points back up
    else {
        // play last card
        currState->playCard(card, position);

        // tally up points

        return heroPts;
    }
    // Copy game state

    // Copy player into new instance

    // Recursively call makePlay for next player with all possible hands they could have

}
*/

