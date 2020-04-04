#include "game.h"

game::game(int buyin) : buyIn(buyin) {

    for (int i = 0; i < nbPlayers; i++) {
        player *pl = new player();
        cout << "Enter name of player " << i + 1 << " : ";
        getline(cin, pl->name);
        pl->bankRoll = buyIn;
        pl->pendingBet = 0;
        players.push_back(pl);
    }

    bigBlind.first = 10;
    bigBlind.second = 0;
    smallBlind.first = 5;
    smallBlind.second = 1;

    deck = new int[52];
    for(int i = 0; i < 52; i++)	deck[i] = i;
    shuffle(deck);

    for(int i = 0; i < nbPlayers; i++){
        stillPlaying.push_back(i);
        players[i]->pendingBet = bigBlind.first;
    }

    communityCards = new int[5];

}

void game::dealPlayersCards(){

    for(int i = 0; i < nbPlayers; i++){
        players[i]->hand.first = deck[indexDeck];
        players[i]->hand.second = deck[indexDeck + 1];
        indexDeck += 2;
        cout << players[i]->name << "'s hand : " << readHand(players[i]->hand.first) << " and " << readHand(players[i]->hand.second) << endl;
    }

}

void game::collectBets(){

    int index = bigBlind.second - 1;
    index %= stillPlaying.size();

    int action;
    int cleared = 0;

    while(cleared < stillPlaying.size() || players[stillPlaying[index]]->pendingBet != 0){

        cout << players[stillPlaying[index]]->name << "'s turn (0 to call or check, 1 to fold, 2 to raise) : ";
        cin >> action;

        switch(action){

            case CALL_OR_CHECK:

                players[stillPlaying[index]]->bankRoll -= players[stillPlaying[index]]->pendingBet;
                pot += players[stillPlaying[index]]->pendingBet;
                players[stillPlaying[index]]->pendingBet = 0;
                break;

            case FOLD:

                players[stillPlaying[index]]->pendingBet = 0;
                stillPlaying.erase(stillPlaying.begin() + index);
                cleared--;
                break;

            case RAISE:

                int plRaise;
                do{

                    cout << "Enter the amount : ";
                    cin >> plRaise;
                    if(plRaise < players[stillPlaying[index]]->pendingBet + bigBlind.first) cout << "Try Again!" << endl;
                    else    break;

                }while(true);

                for(int i = 0; i < stillPlaying.size(); i++)	players[stillPlaying[i]]->pendingBet = plRaise;
                players[stillPlaying[index]]->pendingBet = 0;
                players[stillPlaying[index]]->bankRoll -= plRaise;
                pot += plRaise;
                break;

        }

        if(stillPlaying.size() == 1)	break;
        cleared++;
        index--;
        index %= stillPlaying.size();

    }

}

void game::evaluateRound(){

    int* cards = new int[7];
    int maxId = 0;
    int maxVal = -1;

    if(stillPlaying.size() != 1){

        for(int i = 0; i < 5; i++)	cards[i] = communityCards[i];

        for(int i = 0; i < stillPlaying.size(); i++){

            cards[5] = players[stillPlaying[i]]->hand.first;
            cards[6] = players[stillPlaying[i]]->hand.second;

            int plScore = checkHand(cards);

            if(plScore > maxVal){

                maxId = stillPlaying[i];
                maxVal = plScore;

            } else if(plScore == maxVal){

                if((cards[5] + cards[6])%13 > (players[maxId]->hand.first + players[maxId]->hand.second)%13){

                    maxId = stillPlaying[i];
                    maxVal = plScore;

                }

            }

        }

    } else {

        maxId = stillPlaying[0];

    }
    cout << players[maxId]->name << " won." << endl;
    players[maxId]->bankRoll += pot;
    pot = 0;

    bigBlind.second++;
    bigBlind.second %= nbPlayers;

    smallBlind.second++;
    smallBlind.second %= nbPlayers;

}

void game::nextRound(){

    players[bigBlind.second]->bankRoll -= bigBlind.first;
    players[bigBlind.second]->pendingBet -= bigBlind.first;

    players[smallBlind.second]->bankRoll -= smallBlind.first;
    players[smallBlind.second]->pendingBet -= smallBlind.first;

    pot += bigBlind.first + smallBlind.first;

    dealPlayersCards();

    collectBets();

    if(stillPlaying.size() != 1) {

        for (int i = 0; i < 5; i++) {

            communityCards[i] = deck[indexDeck];

            if (i >= 2) {

                for (int j = 0; j < i; j++) cout << readHand(communityCards[j]) << " and ";
                cout << readHand(communityCards[i]) << endl;

                collectBets();

            }

            if (stillPlaying.size() == 1) break;
            indexDeck++;

        }

    }

    evaluateRound();

    round++;

}