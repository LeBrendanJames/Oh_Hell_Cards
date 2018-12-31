// Want to have two modes - straight simulation and a simulation where everyone's plays can be input as we go
// So, version 1 is just one that spits out a bid and then all of the gameplay
// And version 2 is one that takes input for each other player's bids and plays and spits out recommendations
// for hero only.

#ifndef CARDS_GAME_H
#define CARDS_GAME_H

class Game{
public:
    Game(int numPlayers, int heroPosition, int totalCards, Card * flippedCard, Card ** heroHand);
    ~Game();

    void simRound(); // Follows along with live game and makes recommendations each time it is hero's turn ot act
    void nextRound(); // Moves dealer pointer and sets everyone's positions

private:
    GameState * gmState;
    Player * hero;
    Player * dealer;

    void playCard();
};

#endif //CARDS_GAME_H