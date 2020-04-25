#include <iostream>
#include "controller.h"

int main() {

    controller poker(1000);

    int i = 0;
    while(i < 5){
	poker.nextRound();
	i++;
    }
    return 0;
}
