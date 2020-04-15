#include "cards2.h"

hand::hand(int* cards, int nbCards){

	binary52 = new int[52];
	for(int i = 0; i < 52; i++)	binary52[i] = 0;
	for(int i = 0; i < nbCards; i++)	binary52[cards[i]] = 1;
/*
	quinary13 = new int[13];
	for(int i = 0; i < 13; i++)	quinary13[i] = 0;
	for(int i = 0; i < nbCards; i++)	quinary13[cards[i]%13]++;
*/
}

int hand::flush(){

	int winnerSuit = -1;
	for(int i = 0; i < 4; i++){
		int counter = 0;
		for(int j = 0; i < 13; i++)	if(binary52[(13*i) + j] == 1)	counter++;
		if(counter >= 5){
			winnerSuit = i;
			break;
		}
	}

	return winnerSuit;
}

int hand::straight(int flushResult){
	
	int counter = 0;
	for(int i = 13*(flushResult+1) - 1; i >= 13*flushResult; i--){
		if(binary52[i] == 1)	counter++;
		else	counter = 0;
		if(counter >= 5)	return i+5;
	}
	return -1;

}

int royalFlush

int hand::evaluate(){

	int flushResult = flush();
	if(flushResult == -1){
	
	}else{
		int counter;
		for(int i = 13*(flushResult+1) - 1; i > -1; i--){
			if(binary52[i])
		}
	}

}
