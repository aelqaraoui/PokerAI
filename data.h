#ifndef POKERAI_DATA_H
#define POKERAI_DATA_H

#include <string>
#include <bits/stdc++.h>

typedef struct _player{

	std::string name;
	int bankRoll;
	std::pair<int, int> hand;
	int pendingBet;

}player;

class data {
public:
	int buyIn;
	int indexDeck;
	int *deck;
	int *communityCards;
	std::vector<player> players;
	std::vector<int> stillPlaying;
	std::pair<int, int> bigBlind;
	std::pair<int, int> smallBlind;
	int pot;
	int nbPlayers;

	data(int buyin);
	~data();

};


#endif //POKERAI_DATA_H
