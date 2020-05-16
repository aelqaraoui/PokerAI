#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include "data.h"
#include "interface.h"
#include "pokerAI.h"
#include "cards.h"

using namespace std;

class controller{
private:

    data gameData;
    interface gameInterface;

public:

    controller(int buyin);
    ~controller();
    void initializeRound();
    void dealPlayersCards();
    void collectBets(int nbCardsDealt);
    void evaluateRound();
    bool nextRound();

};

#endif // CONTROLLER_H
