#include <iostream>
#include "game.h"

int main() {

    game poker(1000);

    int i = 0;
    while(i < 5){
	poker.nextRound();
	i++;
    }
    return 0;
}
