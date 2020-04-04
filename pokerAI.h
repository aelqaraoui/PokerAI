#ifndef POKERAI_POKERAI_H
#define POKERAI_POKERAI_H

#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include "cards.h"

#define CALL_OR_CHECK 0
#define FOLD 1
#define RAISE 2

using namespace std;

double potOdds(int bet, int pot);

double rateReturn(int* cards, int nbCards, double potOdds);

int proba(double fold, double call_or_check, double raise);

int FCR(int* cards, int nbCards, int bet, double potOdds);

int pokerAI(int* cards, int nbCards, int bet, int pot);

#endif //POKERAI_POKERAI_H