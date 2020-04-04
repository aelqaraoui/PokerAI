#include "cards.h"

string readHand(int card){

    string result = "";
    switch(card%13){
        case 12:
            result += "Ace ";
            break;
        case 11:
            result += "King ";
            break;
        case 10:
            result += "Queen ";
            break;
        case 9:
            result += "Jack ";
            break;
        default:
            result += to_string((card%13)+2 ) + " ";
            break;
    }
    switch(card/13){
        case 0:
            result += "of Clubs";
            break;
        case 1:
            result += "of Diamonds";
            break;
        case 2:
            result += "of Hearts";
            break;
        case 3:
            result += "of Spades";
            break;
    }
    return result;
}

void shuffle(int* deck){

    int n = 52;
    srand(time(NULL));
    for(int i = n-1; i > 0; i--){
        int j = rand()%(i+1);
        if(i == j) continue;
        deck[i] = deck[i] + deck[j];
        deck[j] = deck[i] - deck[j];
        deck[i] = deck[i] - deck[j];
    }

}

bool royalFlush(int* cards, int nbCards){

    int* nbSuits = new int[4];
    for(int i = 0; i < 4; i++)	nbSuits[i] = 0;

    for(int i = 0; i < nbCards; i++)	nbSuits[(int)(cards[i]/13)]++;

    int maxSuit = 0;
    for(int i = 1; i < 4; i++)	if(nbSuits[i] > nbSuits[maxSuit])	maxSuit = i;
    if(nbSuits[maxSuit] < 5)	return false;

    int* same = new int[(int)nbSuits[maxSuit]];   // the biggest set of same suit cards
    int id = 0;
    for(int i = 0; i < nbCards; i++)	if((cards[i]/13) == maxSuit){ same[id] = cards[i]%13; id++; };
    sort(same, same + nbSuits[maxSuit], greater<int>());
    if(same[0] != 12){
        delete nbSuits;
        delete same;
        return false;
    }
    for(int i = 0; i < 4; i++)	if((same[i]-same[i+1]) != 1){
            delete nbSuits;
            delete same;
            return false;
        }
    delete nbSuits;
    delete same;
    return true;

}

bool straightFlush(int* cards, int nbCards){

    int* nbSuits = new int[4];
    for(int i = 0; i < 4; i++)	nbSuits[i] = 0;

    for(int i = 0; i < nbCards; i++)	nbSuits[(int)(cards[i]/13)]++;

    int maxSuit = 0;
    for(int i = 1; i < 4; i++)	if(nbSuits[i] > nbSuits[maxSuit])	maxSuit = i;
    if(nbSuits[maxSuit] < 5)	return false;

    int* same = new int[(int)nbSuits[maxSuit]];   // the biggest set of same suit cards
    int id = 0;
    for(int i = 0; i < nbCards; i++)	if((cards[i]/13) == maxSuit){ same[id] = cards[i]%13; id++; };
    sort(same, same + nbSuits[maxSuit], greater<int>());

    int result;
    for(int j = 0; j < 3; j++){
        result = true;
        for(int i = j; i < j+4; i++)	result = result && ((same[i]-same[i+1]) == 1);
        if(result)	break;
    }
    delete nbSuits;
    delete same;
    return result;

}

bool kind(int* cards, int n, int nbCards){

    int* nbRanks = new int[13];
    for(int i = 0; i < 13; i++)	nbRanks[i] = 0;

    for(int i = 0; i < nbCards; i++)	nbRanks[(int)(cards[i]%13)]++;

    int maxRank = 0;
    for(int i = 1; i < 13; i++)	if(nbRanks[i] > nbRanks[maxRank])	maxRank = i;
    if(nbRanks[maxRank] < n){
        delete nbRanks;
        return false;
    }
    delete nbRanks;
    return true;

}

bool fullHouse(int* cards, int nbCards){

    int* nbRanks = new int[13];
    for(int i = 0; i < 13; i++)	nbRanks[i] = 0;

    for(int i = 0; i < nbCards; i++)	nbRanks[(int)(cards[i]%13)]++;
    sort(nbRanks, nbRanks+13, greater<int>());

    delete nbRanks;
    return (nbRanks[0] >= 3 && nbRanks[1] >= 2);

}


bool twoPairs(int* cards, int nbCards){

    int* nbRanks = new int[13];
    for(int i = 0; i < 13; i++)	nbRanks[i] = 0;

    for(int i = 0; i < nbCards; i++)	nbRanks[(int)(cards[i]%13)]++;
    sort(nbRanks, nbRanks+13, greater<int>());

    if(nbRanks[0] >= 2 && nbRanks[1] >= 2){
        delete nbRanks;
        return true;
    }else{
        delete nbRanks;
        return true;
    }
}

bool flush(int* cards, int nbCards){

    int* nbSuits = new int[4];
    for(int i = 0; i < 4; i++)	nbSuits[i] = 0;

    for(int i = 0; i < nbCards; i++)	nbSuits[(int)(cards[i]/13)]++;

    int maxSuit = 0;
    for(int i = 1; i < 4; i++)	if(nbSuits[i] > nbSuits[maxSuit])	maxSuit = i;
    if(nbSuits[maxSuit] < 5){
        delete nbSuits;
        return false;
    }
    delete nbSuits;
    return true;

}

bool next_combination(int* item, int n, int N)
{
    for (int i = 1; i <= n; ++i) {
        if (item[n-i] < N-i) {
            ++item[n-i];
            for (int j = n-i+1; j < n; ++j) {
                item[j] = item[j-1] + 1;
            }
            return true;
        }
    }
    return false;
}

int removeDuplicates(int* arr, int n){

    if (n==0 || n==1)
        return n;

    int j = 0;

    for (int i=0; i < n-1; i++)
        if (arr[i] != arr[i+1])	arr[j++] = arr[i];

    arr[j++] = arr[n-1];

    return j;

}

bool straight(int* cards, int nbCards){

    int* ranks = new int[nbCards];
    for(int i = 0; i < nbCards; i++)	ranks[i] = cards[i]%13;
    sort(ranks, ranks + nbCards);
    nbCards = removeDuplicates(ranks, nbCards);
    vector<int> occ;
    for(int i = 0; i < nbCards-1; i++){

        int counter = 1;
        do{ counter++; i++; }
        while(ranks[i+1]-ranks[i] == 1 && i+1 < nbCards);
        occ.push_back(counter);

    }
    sort(occ.begin(), occ.end(), greater<int>());
    delete ranks;
    if(occ[0] >= 5){
        vector<int>().swap(occ);
        return true;
    }else{
        vector<int>().swap(occ);
        return false;
    }

}

int checkHand(int* cards, int nbCards){

    if(nbCards >= 5){
        if(royalFlush(cards, nbCards))	return ROYALFLUSH * 52;
        if(straightFlush(cards, nbCards))	return STRAIGHTFLUSH * 52 + ((cards[0]%13) + (cards[1]%13));
        if(kind(cards, 4, nbCards))	return FOUROFAKIND * 52 + ((cards[0]%13) + (cards[1]%13));
        if(fullHouse(cards, nbCards))	return FULLHOUSE * 52 + ((cards[0]%13) + (cards[1]%13));
        if(flush(cards, nbCards))	return FLUSH * 52 + ((cards[0]%13) + (cards[1]%13));
        if(straight(cards, nbCards))	return STRAIGHT * 52 + ((cards[0]%13) + (cards[1]%13));
        if(kind(cards, 3, nbCards))	return THREEOFAKIND * 52 + ((cards[0]%13) + (cards[1]%13));
        if(twoPairs(cards, nbCards))	return TWOPAIRS * 52 + ((cards[0]%13) + (cards[1]%13));
        if(kind(cards, 2, nbCards))	return PAIR * 52 + ((cards[0]%13) + (cards[1]%13));
        return HIGHCARD + ((cards[0]%13) + (cards[1]%13))*((int)(cards[0]/13 == cards[1]/13) + 1);
    }

}