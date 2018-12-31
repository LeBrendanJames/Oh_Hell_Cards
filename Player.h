// Player class contains information only available to the player (so, their hand)
// Game class will then ask player questions (how much to bid, what to play) by passing in the gameState and
// waiting for a response

#ifndef CARDS_PLAYER_H
#define CARDS_PLAYER_H

#include <iostream>
#include <vector>
#include <random>
#include "Card.h"
#include "GameState.h"

class Player{
public:
    Player(bool isHero, int position);
    ~Player() = default;

    bool isHero();
    int getPosition();
    void setHand(GameState * currState, std::string * inputCards = nullptr);
    void printHand();

    int makeBid(GameState * currState);
    Card * makePlay(GameState * currState); // This is seperate from evaluatePlays so that I can implement skill levels
                                            // Play made not necessarily = optimal play if player not expert

private:
    bool hero;
    int position;
    std::vector<Card> plyrHand;
    GameState * gmState;

    double simDecision(GameState * currState, Card * cardPlayed);
};


#endif //CARDS_PLAYER_H