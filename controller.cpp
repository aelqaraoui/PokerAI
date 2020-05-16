#include "controller.h"

controller::controller(int buyin) : gameData(buyin), gameInterface(800, 600){

    player pl;
    pl.name = "Amine";
    pl.bankRoll = buyin;
    gameData.players.push_back(pl);

    pl.name = "Computer";
    pl.bankRoll = buyin;
    gameData.players.push_back(pl);

    for(int i = 0; i < 52; i++)	gameData.deck[i] = i;

}

controller::~controller(){



}

void controller::initializeRound(){

    for(int i = 0; i < 52; i++)	gameData.deck[i] = i;
    gameData.indexDeck = 0;
    shuffle(gameData.deck);

    for(int i = 0; i < gameData.nbPlayers; i++)
	gameData.players[i].pendingBet = gameData.bigBlind.first; 

    gameData.players[gameData.bigBlind.second].pendingBet -= gameData.bigBlind.first;
    gameData.players[gameData.bigBlind.second].bankRoll -= gameData.bigBlind.first;

    gameData.players[gameData.smallBlind.second].pendingBet -= gameData.smallBlind.first;
    gameData.players[gameData.smallBlind.second].bankRoll -= gameData.smallBlind.first;
   
    gameData.pot += gameData.bigBlind.first + gameData.smallBlind.first;

    gameData.stillPlaying.clear();
   
    for(int i = 0; i < gameData.nbPlayers; i++)
        gameData.stillPlaying.push_back(i);

}

void controller::dealPlayersCards(){


    for(int i = 0; i < gameData.nbPlayers; i++){
        gameData.players[i].hand.first = gameData.deck[gameData.indexDeck];

	gameData.players[i].hand.second = gameData.deck[gameData.indexDeck + 1];

	gameData.indexDeck += 2;
    }
    gameInterface.setPlayerHand(gameData.players[0].hand);
	
}

void controller::collectBets(int nbCardsDealt){

    int index = gameData.bigBlind.second - 1;
    index %= gameData.stillPlaying.size();

    int action;
    int cleared = 0;

    while(cleared < (int)gameData.stillPlaying.size() && gameData.stillPlaying.size() != 1){

        int plRaise = 0;

        if(gameData.players[gameData.stillPlaying[index]].name != "Computer") {

            gameInterface.setAction();
            action = gameInterface.getAction();

        }else{

            int* cards = new int[nbCardsDealt+2]();
            for(int i = 0; i < nbCardsDealt; i++)   cards[i] = gameData.communityCards[i];
            cards[nbCardsDealt] = gameData.players[gameData.stillPlaying[index]].hand.first;
            cards[nbCardsDealt+1] = gameData.players[gameData.stillPlaying[index]].hand.second;
            action = pokerAI(cards, nbCardsDealt+2, gameData.players[gameData.stillPlaying[index]].pendingBet, gameData.pot);
            plRaise = 2 * gameData.players[gameData.stillPlaying[index]].pendingBet;
            delete[] cards;

        }

	gameInterface.resetButtons();

        switch(action){

            case CALL_OR_CHECK:

                cout << gameData.players[gameData.stillPlaying[index]].name << " Called/Checked."<< endl;
		gameData.players[gameData.stillPlaying[index]].bankRoll -= gameData.players[gameData.stillPlaying[index]].pendingBet;
                gameData.pot += gameData.players[gameData.stillPlaying[index]].pendingBet;
                gameData.players[gameData.stillPlaying[index]].pendingBet = 0;
                break;

            case FOLD:

         	cout << gameData.players[gameData.stillPlaying[index]].name << " Folded."<< endl;
                gameData.players[gameData.stillPlaying[index]].pendingBet = 0;

                gameData.stillPlaying.erase(gameData.stillPlaying.begin() + index);
	
                cleared--;
                break;

            case RAISE:

                if(gameData.players[gameData.stillPlaying[index]].name != "Computer")    do{

                    gameInterface.setRaise();
                    plRaise = gameInterface.getRaise();

                }while(plRaise < gameData.players[gameData.stillPlaying[index]].pendingBet + gameData.bigBlind.first);

 		        cout << gameData.players[gameData.stillPlaying[index]].name << " Raised to " << plRaise << "." << endl;
		        int pl = plRaise - gameData.players[gameData.stillPlaying[index]].pendingBet;
		        for(int i = 0; i < (int)gameData.stillPlaying.size(); i++)	gameData.players[gameData.stillPlaying[i]].pendingBet += pl;
		
		        gameData.players[gameData.stillPlaying[index]].pendingBet = 0;
                	gameData.players[gameData.stillPlaying[index]].bankRoll -= plRaise;
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

            cards[5] = gameData.players[gameData.stillPlaying[i]].hand.first;
            cards[6] = gameData.players[gameData.stillPlaying[i]].hand.second;

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

    gameInterface.setOpponentHand(gameData.players[1].hand);
    gameInterface.celebrateWinner(gameData.players[maxId].name, gameData.pot);
    gameData.players[maxId].bankRoll += gameData.pot;
    gameData.pot = 0;

    gameData.bigBlind.second++;
    gameData.bigBlind.second %= gameData.nbPlayers;

    gameData.smallBlind.second++;
    gameData.smallBlind.second %= gameData.nbPlayers;

}

bool controller::nextRound(){

    cout << "################# NEW ROUND ################# " << endl;	
    
    initializeRound();

    for(int i = 0; i < (int)gameData.stillPlaying.size(); i++){
    	cout << gameData.players[gameData.stillPlaying[i]].name << "'s bankroll : " << gameData.players[gameData.stillPlaying[i]].bankRoll << endl;
    }

    dealPlayersCards();

    collectBets(0);

    if(gameData.stillPlaying.size() != 1) {

        for (int i = 0; i < 5; i++) {

            gameData.communityCards[i] = gameData.deck[gameData.indexDeck];
	    gameInterface.addCommunityCard(gameData.communityCards[i]);

            if (i >= 2) {

                collectBets(i+1);

            }

	    if(gameData.stillPlaying.size() == 1)	break;
	   
            gameData.indexDeck++;

        }

    }


    evaluateRound();
    bool pl = (gameInterface.getReplay() == 0);
    gameInterface.resetGame();
    return pl;
}
