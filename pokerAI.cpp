#include "pokerAI.h"

double potOdds(int bet, int pot){

    return (double)pot/(double)(pot + bet);

}

double rateReturn(int* cards, int nbCards, double potOdds){

    double HS = 0;
    int nbGames = 0;
    if(nbCards == 2){

        ifstream handStrength("./decision/" + to_string(min(cards[0], cards[1])) + "_" + to_string(max(cards[0], cards[1])) + ".txt");
        string line;
        getline(handStrength, line);
        HS = stod(line);
        getline(handStrength, line);
        nbGames = stoi(line);
        handStrength.close();

    }else{

        int* deck = new int[52-nbCards]();
        int indexDeck = 0;

        for(int i = 0; i < 52; i++){
            int truth = true;
            for(int j = 0; j < nbCards; j++)    if(i == cards[j]){   truth = false; break;   }
            if(truth){
                deck[indexDeck] = i;
                indexDeck++;
            }
        }

        int* plcards = new int[9]();
        for(int i = 0; i < nbCards; i++)    plcards[i] = cards[i];
        
	int n = 9 - nbCards;
        int N = 52 - nbCards;
        int* item = new int[n]();
        for (int i = 0; i < n; i++) item[i] = i;
        int nb = 0;
        int wins = 0;

        do{

            for(int i = nbCards; i < n; i++)  plcards[nbCards+i] = deck[item[i]];

            do{

                if(checkHand(plcards) > checkHand(plcards + 2))   wins++;
                nb++;

            }while(next_permutation(plcards + n, plcards + 9));

        }while(next_combination(item, n, N));

        HS = wins;
        nbGames = nb;

	delete[] deck;
	delete[] plcards;
	delete[] item;

    }

    HS = HS/nbGames;

    return HS * potOdds;

}

int proba(double fold, double call_or_check, double raise){

    int randomNb = rand()%100;
    if(randomNb <= (int)(fold*100)) return FOLD;
    else if(randomNb <= (int)((fold + call_or_check)*100))  return CALL_OR_CHECK;
    else    return RAISE;

}

int FCR(int* cards, int nbCards, int bet, double potOdds){

    double rr = rateReturn(cards, nbCards, potOdds);
    int result;

    if(rr < 0.8)    result = proba(0.95, 0, 0.05);
    else if(rr < 1.0)   result = proba(0.8, 0.05, 0.15);
    else if(rr < 1.3)   result = proba(0, 0.6, 0.4);
    else    result = proba(0, 0.3, 0.7);

    if(result == FOLD && bet == 0)  return CALL_OR_CHECK;
    return result;

}

int pokerAI(int* cards, int nbCards, int bet, int pot){

    srand(time(NULL));
    double po = potOdds(bet, pot);

    return FCR(cards, nbCards, bet, po);

}
