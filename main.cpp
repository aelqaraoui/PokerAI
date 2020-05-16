#include <iostream>
#include "controller.h"

int main() {

	controller poker(1000);

	while(poker.nextRound());

/*
	interface gameInterface(640, 480);

	std::pair<int, int> pl;
	pl.first = 5;
	pl.second = 0;
	gameInterface.setPlayerHand(pl);
	gameInterface.setAction();
*/
    	return 0;

}
