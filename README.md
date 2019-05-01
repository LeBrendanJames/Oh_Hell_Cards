# Oh_Hell_Cards
This program is a utility that recommends optimal plays for the 'Oh Hell' card game.
The game is a variation of the common trick-taking card game style. Players are dealt a hand,
then decide on a bid of how many tricks they expect to win. Points are then accumulated based on
how many tricks each player wins throughout the hand, with a bonus at the conclusion of the hand
if a player's bid matches the number of tricks they have won. 
More information can be found at: https://en.wikipedia.org/wiki/Oh_Hell

Note: I borrow from poker terminology and use the term 'Hero' to describe the player the program
is making recommendations for, both in this readme and in the source code itself.

Compilation/Running the Program:
The program includes a makefile, so, when in the directory with the Makefile,
just type 'make'.
Once the program has compiled, you can run the program with the command './ohHell'.

Gameplay:
The program has two main capabilities:
1. Find the optimal move in a single situation
2. Follow along with a game in progress and make recommendations each time it is the player in question's turn.

These options can be selected at the outset of the game, and the program will then gather information on
the current state of the game.  

Technical Details (High-level):
The program has two main classes: GameState and DecisionPoint
Information is gathered either in the ohHell.cpp main program or throughout
a game object. The primary purpose of these is to set up a GameState object, 
which holds all of the information on the state of the current game (number of players,
cards remaining, hero position, hero's hand, etc.). The gamestate is updated as plays are made.
When the game reaches a point where the hero needs to make a play, the gamestate is passed into a
DecisionPoint object. This object makes a copy of the gamestate and plays out all possible variations
of the game, returning whichever move results in the highest expected points.

Technical Details (Accuracy of Recommendations):
Oh Hell is, in theory, solvable, but the number of permutations grows at a rate that makes solving
even mildly complicated situations infeasible. Therefore, the DecisionPoint class is built to perform
Monte Carlo simulation until it has a recommendation that it is reasonably satisfied with. There is no 
guarantee that this is the correct recommendation, but after simulations, it is the best move at a high 
enough frequency where the DecisionPoint class decides to return it as the recommended play.