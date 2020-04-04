#include <iostream>
#include "pokerAI.h"

int main() {

    game poker(100);

    poker.nextRound();


    int cards[] = {0, 1, 3, 4, 2};

    cout << pokerAI(cards, 5, 10, 50) << endl;

    return 0;
}
