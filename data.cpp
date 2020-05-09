#include "data.h"

data::data(int buyin) : buyIn(buyin){

	nbPlayers = 2;
	deck = new int[52];
	communityCards = new int[5];
	pot = 0;
	
	bigBlind.first = 20;
	bigBlind.second = 0;

	smallBlind.first = 10;
	smallBlind.second = 1;

}

data::~data(){

	delete[] deck;
	delete[] communityCards;

}
