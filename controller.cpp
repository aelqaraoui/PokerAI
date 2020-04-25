#include "controller.h"

controller::controller(int buyin) : gameData(buyin), gameInterface(){

    player pl;
    pl.name = gameInterface.getPlayerName();
    pl.bankRoll = buyIn;
    gameData.players.push_back(pl);

    pl.name = "Computer";
    pl.bankRoll = buyIn;
    gameData.players.push_back(pl);

    deck = new int[52];
    for(int i = 0; i < 52; i++)	deck[i] = i;

    communityCards = new int[5];

}

controller::~controller(){

    delete []deck;
    delete []communityCards;

}

void controller::initializeRound(){

    gameData.indexDeck = 0;
    shuffle(gameData.deck);

    for(int i = 0; i < gameData.nbPlayers; i++){
    
	gameData.players[i].pendingBet = gameData.bigBlind.first;
    
    } 

    gameData.players[bigBlind.second].pendingBet -= gameData.bigBlind.first;
    gameData.players[bigBlind.second].bankRoll -= gameData.bigBlind.first;

    gameData.players[smallBlind.second].pendingBet -= gameData.smallBlind.first;
    gameData.players[smallBlind.second].bankRoll -= gameData.smallBlind.first;
   
    gameData.pot += gameData.bigBlind.first + gameData.smallBlind.first;

    gameData.stillPlaying.clear();
   
    for(int i = 0; i < gameData.nbPlayers; i++){
        gameData.stillPlaying.push_back(i);
    }

}

void controller::dealPlayersCards(){

    for(int i = 0; i < gameData.nbPlayers; i++){
        gameData.players[i].hand.first = gameData.deck[indexDeck];
        gameData.players[i].hand.second = gameData.deck[indexDeck + 1];
	    gameData.indexDeck += 2;
    }
    gameInterface.showPlayerHand(gameData.players[0].hand);
	
}

void controller::collectBets(int nbCardsDealt){

    int index = gameData.bigBlind.second - 1;
    index %= gameData.stillPlaying.size();

    int action;
    int cleared = 0;

    while(cleared < (int)stillPlaying.size() && stillPlaying.size() != 1){

        int plRaise = 0;	

        if(gameData.players[stillPlaying[index]].name != "Computer") {

            gameInterface.setAction();
            action = gameInterface.getAction();

        }else{

            int* cards = new int[nbCardsDealt+2]();
            for(int i = 0; i < nbCardsDealt; i++)   cards[i] = gameData.communityCards[i];
            cards[nbCardsDealt] = gameData.players[stillPlaying[index]].hand.first;
            cards[nbCardsDealt+1] = gameData.players[stillPlaying[index]].hand.second;
            action = pokerAI(cards, nbCardsDealt+2, gameData.players[stillPlaying[index]].pendingBet, gameData.pot);
            plRaise = 2 * gameData.players[stillPlaying[index]].pendingBet;
            delete[] cards;

        }

        switch(action){

            case CALL_OR_CHECK:

                cout << gameData.players[stillPlaying[index]].name << " Called/Checked."<< endl;
		        gameData.players[stillPlaying[index]].bankRoll -= gameData.players[stillPlaying[index]].pendingBet;
                gameData.pot += gameData.players[stillPlaying[index]].pendingBet;
                gameData.players[stillPlaying[index]].pendingBet = 0;
                break;

            case FOLD:

         		cout << gameData.players[stillPlaying[index]].name << " Folded."<< endl;
                gameData.players[stillPlaying[index]].pendingBet = 0;

                gameData.stillPlaying.erase(stillPlaying.begin() + index);
	
                cleared--;
                break;

            case RAISE:

                if(gameData.players[stillPlaying[index]].name != "Computer")    do{

                    gameInterface.setRaise();
                    plRaise = gameInterface.getRaise();

                }while(plRaise < gameData.players[stillPlaying[index]].pendingBet + gameData.bigBlind.first);

 		        cout << gameData.players[stillPlaying[index]].name << " Raised to " << plRaise << "." << endl;
		        int pl = plRaise - gameData.players[stillPlaying[index]].pendingBet;
		        for(int i = 0; i < (int)gameData.stillPlaying.size(); i++)	gameData.players[stillPlaying[i]].pendingBet += pl;
		
		        gameData.players[stillPlaying[index]].pendingBet = 0;
                gameData.players[stillPlaying[index]].bankRoll -= plRaise;
                gameData.pot += plRaise;
		        cleared = 0;
                break;

        }
	    cout << "POT : " << gameData.pot << endl;

        if(gameData.stillPlaying.size() == 1)	break;
        cleared++;
        ++index;
        index %= gameData.stillPlaying.size();

    }

}

void controller::evaluateRound(){

    int* cards = new int[7]();
    int maxId = 0;
    int maxVal = -1;

    if(gameData.stillPlaying.size() != 1){

        for(int i = 0; i < 5; i++)	cards[i] = gameData.communityCards[i];

        for(int i = 0; i < (int)gameData.stillPlaying.size(); i++){

            cards[5] = gameData.players[stillPlaying[i]].hand.first;
            cards[6] = gameData.players[stillPlaying[i]].hand.second;

            int plScore = checkHand(cards);

            if(plScore > maxVal){

                maxId = gameData.stillPlaying[i];
                maxVal = plScore;

            } else if(plScore == maxVal){

                if((cards[5] + cards[6])%13 > (gameData.players[maxId].hand.first + gameData.players[maxId].hand.second)%13){

                    maxId = gameData.stillPlaying[i];
                    maxVal = plScore;

                }

            }

        }

    } else {

        maxId = gameData.stillPlaying[0];

    }

    delete []cards;

    gameInterface.showOpponentHands();
    gameInterface.celebrateWinner(gameData.players[maxId].name, gameData.pot);
    gameData.players[maxId].bankRoll += gameData.pot;
    gameData.pot = 0;

    gameData.bigBlind.second++;
    gameData.bigBlind.second %= gameData.nbPlayers;

    gameData.smallBlind.second++;
    gameData.smallBlind.second %= gameData.nbPlayers;

}

void controller::nextRound(){

    cout << "################# NEW ROUND ################# " << endl;

    initializeRound();

    for(int i = 0; i < (int)gameData.stillPlaying.size(); i++){
    	cout << gameData.players[stillPlaying[i]].name << "'s bankroll : " << gameData.players[stillPlaying[i]].bankRoll << endl;
    }

    dealPlayersCards();

    collectBets(0);

    if(stillPlaying.size() != 1) {

        for (int i = 0; i < 5; i++) {

            gameData.communityCards[i] = gameData.deck[indexDeck];

            if (i >= 2) {

                gameInterface.showCommunityCards(gameData.communityCards, i);

                collectBets(i+1);

            }

	    if(gameData.stillPlaying.size() == 1)	break;
	   
            gameData.indexDeck++;

        }

    }

    evaluateRound();

}
