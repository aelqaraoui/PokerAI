#ifndef POKERAI_CARDS_H
#define POKERAI_CARDS_H

#include <iostream>
#include <bits/stdc++.h>

#define HIGHCARD 0
#define PAIR 1
#define TWOPAIRS 2
#define THREEOFAKIND 3
#define STRAIGHT 4
#define FLUSH 5
#define FULLHOUSE 6
#define FOUROFAKIND 7
#define STRAIGHTFLUSH 8
#define ROYALFLUSH 9

using namespace std;

string readHand(int card);

void shuffle(int* deck);

bool royalFlush(int* cards, int nbCards = 7);

bool straightFlush(int* cards, int nbCards = 7);

bool kind(int* cards, int n, int nbCards = 7);

bool fullHouse(int* cards, int nbCards = 7);

bool twoPairs(int* cards, int nbCards = 7);

bool flush(int* cards, int nbCards = 7);

bool next_combination(int* item, int n, int N);

int removeDuplicates(int* arr, int n);

bool straight(int* cards, int nbCards = 7);

int checkHand(int* cards, int nbCards = 7);

#endif //POKERAI_CARDS_H