#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

int assertTrue(struct gameState* testG, int* players);

int main() {

	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;
	int nextPlayer = 1;
	int players[4]; 
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	
	printf("----------------- Testing Function: getWinners() ----------------\n");

	//-------------------TEST 1: 2-player game, player 1 wins----------------------
	printf("TEST 1: 2-player game, player 1 wins\n");
	for(int i = 0; i < 5; i++){
		G.hand[thisPlayer][i] = duchy;
		G.hand[nextPlayer][i] = estate;
		G.handCount[thisPlayer]++;
		G.handCount[nextPlayer]++;
		G.deck[thisPlayer][i] = duchy;
		G.deck[nextPlayer][i] = estate;
		G.deckCount[thisPlayer]++;
		G.deckCount[nextPlayer]++;
		G.discard[thisPlayer][i] = duchy;
		G.discard[nextPlayer][i] = estate;
		G.discardCount[thisPlayer]++;
		G.discardCount[nextPlayer]++;
	}

	memcpy(&testG, &G, sizeof(struct gameState));
	getWinners(players, &testG);
	assertTrue(&testG, players);

	return 0;
}

int assertTrue(struct gameState* testG, int* players){
	
	int maxPlayers = 4;
	for(int i = 0; i < maxPlayers; i++){
		if(players[i]){
			printf("Player %d is the winner, with a score of %d\n", i+1, scoreFor(i, testG));
		}else{
			printf("Player %d lost or didn't play, with a score of %d\n", i+1, scoreFor(i, testG));
		}
	}

	return 0;
}
