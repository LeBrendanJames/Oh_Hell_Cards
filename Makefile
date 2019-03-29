
game.o: src/game/Game.h src/game/Game.cpp 
	g++ -o game.o -c Game.cpp 

decisionpoint.o: src/decisionpoint/DecisionPoint.h src/decisionpoint/DecisionPoint.cpp
	g++ -c src/decisionpoint/DecisionPoint.cpp 
	
gamestate.o: src/gamestate/gameState.h src/gamestate/gameState.cpp
	g++ -c src/gamestate/gameState.cpp 

card.o: src/card/Card.h src/card/Card.cpp
	g++ -o card.o -c src/card/Card.cpp
	
cardtest: src/card/cardTest.cpp card.o
	g++ -o cardTest src/card/cardTest.cpp card.o

decisionpointtest: src/decisionpoint/decisionpointTest.cpp decisionpoint.o card.o gamestate.o
	g++ -o decisionpointTest src/decisionpoint/decisionpointTest.cpp decisionpoint.o gamestate.o card.o

gamestatetest: src/gamestate/gameState.cpp gamestate.o card.o
	g++ -o gamestateTest src/gamestate/gamestateTest.cpp gamestate.o card.o
	
gametest:

test: cardtest decisionpointtest gamestatetest gametest 
	./cardTest
	./decisionpointTest
	./gamestateTest
	./gameTest

clean:
	rm -f *.o *.exe