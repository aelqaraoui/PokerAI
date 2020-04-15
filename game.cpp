#include "game.h"

game::game(int buyin) : buyIn(buyin) {

    nbPlayers = 2;

    pot = 0;
    bigBlind.first = 10;
    bigBlind.second = 0;
    smallBlind.first = 5;
    smallBlind.second = 1;

    for (int i = 0; i < nbPlayers; i++) {
        player pl;
        cout << "Enter name of player " << i + 1 << " : ";
        getline(cin, pl.name);
        pl.bankRoll = buyIn;
        
        players.push_back(pl);
    }

    deck = new int[52];
    for(int i = 0; i < 52; i++)	deck[i] = i;

    communityCards = new int[5];

}

game::~game(){

    delete []deck;
    delete []communityCards;

}

void game::initializeRound(){

    indexDeck = 0;
    shuffle(deck);

    for(int i = 0; i < nbPlayers; i++){
    
	players[i].pendingBet = bigBlind.first;
    
    } 

    players[bigBlind.second].pendingBet -= bigBlind.first;
    players[bigBlind.second].bankRoll -= bigBlind.first;

    players[smallBlind.second].pendingBet -= smallBlind.first;
    players[smallBlind.second].bankRoll -= smallBlind.first;
   
    pot += bigBlind.first + smallBlind.first;

    stillPlaying.clear(); 
   
    for(int i = 0; i < nbPlayers; i++){
        stillPlaying.push_back(i);
    }

}

void game::dealPlayersCards(){

    for(int i = 0; i < nbPlayers; i++){
        players[i].hand.first = deck[indexDeck];
        players[i].hand.second = deck[indexDeck + 1];        
	indexDeck += 2;

        cout << players[i].name << "'s hand : " << readHand(players[i].hand.first) << " and " << readHand(players[i].hand.second) << endl; 
    }
	
}

void game::collectBets(int nbCardsDealt){

    int index = bigBlind.second - 1;
    index %= stillPlaying.size();

    int action;
    int cleared = 0;

    while(cleared < (int)stillPlaying.size() && stillPlaying.size() != 1){

        int plRaise = 0;	

        if(players[stillPlaying[index]].name != "Computer") {

            cout << players[stillPlaying[index]].name << "'s turn (0 to call or check, 1 to fold, 2 to raise) : ";
            cin >> action;

        }else{

            int* cards = new int[nbCardsDealt+2]();
            for(int i = 0; i < nbCardsDealt; i++)   cards[i] = communityCards[i];
            cards[nbCardsDealt] = players[stillPlaying[index]].hand.first;
            cards[nbCardsDealt+1] = players[stillPlaying[index]].hand.second;
            action = pokerAI(cards, nbCardsDealt+2, players[stillPlaying[index]].pendingBet, pot);
            plRaise = 2*players[stillPlaying[index]].pendingBet;
            delete[] cards;

        }

        switch(action){

            case CALL_OR_CHECK:

                cout << players[stillPlaying[index]].name << " Called/Checked."<< endl;
		players[stillPlaying[index]].bankRoll -= players[stillPlaying[index]].pendingBet;
                pot += players[stillPlaying[index]].pendingBet;
                players[stillPlaying[index]].pendingBet = 0;
                break;

            case FOLD:

 		cout << players[stillPlaying[index]].name << " Folded."<< endl; 
                players[stillPlaying[index]].pendingBet = 0;
                
		stillPlaying.erase(stillPlaying.begin() + index);
	
                cleared--;
                break;

            case RAISE:

                if(players[stillPlaying[index]].name != "Computer")    do{

                    cout << "Enter the amount : ";
                    cin >> plRaise;
                    if(plRaise < players[stillPlaying[index]].pendingBet + bigBlind.first) cout << "Try Again!" << endl;
                    else    break;

                }while(true);

 		cout << players[stillPlaying[index]].name << " Raised to " << plRaise << "." << endl;
		int pl = plRaise - players[stillPlaying[index]].pendingBet;
                for(int i = 0; i < (int)stillPlaying.size(); i++)	players[stillPlaying[i]].pendingBet += pl;
		
		players[stillPlaying[index]].pendingBet = 0;
                players[stillPlaying[index]].bankRoll -= plRaise;
                pot += plRaise;
		cleared = 0;
                break;

        }
	cout << "POT : " << pot << endl;

        if(stillPlaying.size() == 1)	break;
        cleared++;
        ++index;
        index %= stillPlaying.size();

    }

}

void game::evaluateRound(){

    int* cards = new int[7]();
    int maxId = 0;
    int maxVal = -1;

    if(stillPlaying.size() != 1){

        for(int i = 0; i < 5; i++)	cards[i] = communityCards[i];

        for(int i = 0; i < (int)stillPlaying.size(); i++){

            cards[5] = players[stillPlaying[i]].hand.first;
            cards[6] = players[stillPlaying[i]].hand.second;

            int plScore = checkHand(cards);

            if(plScore > maxVal){

                maxId = stillPlaying[i];
                maxVal = plScore;

            } else if(plScore == maxVal){

                if((cards[5] + cards[6])%13 > (players[maxId].hand.first + players[maxId].hand.second)%13){

                    maxId = stillPlaying[i];
                    maxVal = plScore;

                }

            }

        }

    } else {

        maxId = stillPlaying[0];

    }

    delete []cards;

    cout << players[maxId].name << " won " << pot << "." << endl;
    players[maxId].bankRoll += pot;
    pot = 0;

    bigBlind.second++;
    bigBlind.second %= nbPlayers;

    smallBlind.second++;
    smallBlind.second %= nbPlayers;

}

void game::nextRound(){

    cout << "################# NEW ROUND ################# " << endl;

    initializeRound();

    for(int i = 0; i < (int)stillPlaying.size(); i++){
    	cout << players[stillPlaying[i]].name << "'s bankroll : " << players[stillPlaying[i]].bankRoll << endl;
    }

    dealPlayersCards();

    collectBets(0);

    if(stillPlaying.size() != 1) {

        for (int i = 0; i < 5; i++) {

            communityCards[i] = deck[indexDeck];

            if (i >= 2) {

                for (int j = 0; j < i; j++) cout << readHand(communityCards[j]) << " and ";
                cout << readHand(communityCards[i]) << endl;

                collectBets(i+1);

            }

	    if(stillPlaying.size() == 1)	break;
	   
            indexDeck++;

        }

    }

    evaluateRound();

}
