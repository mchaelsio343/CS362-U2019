#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

int assertTrue(struct gameState* testG, struct gameState* G, int shuffleFail, int thisPlayer);

int main() {

	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	printf("----------------- Testing Function: shuffle() ----------------\n");
	//---------------------- TEST 1: deck count = 0 = shuffle fail, return -1 --------------------
	printf("--------------------TEST 1: deck count = 0 = shuffle fail, return -1-----------------------\n");
	for(int i = 0; i < 10; i++){//manually setting entire deck to -1
		G.deck[thisPlayer][i] = -1;
	}	
	G.deckCount[thisPlayer] = 0;//necessary condition for shuffle function to fail
	memcpy(&testG, &G, sizeof(struct gameState));
	int shuffleFail = shuffle(thisPlayer, &testG);
	assertTrue(&testG, &G, shuffleFail, thisPlayer);

	//--------------------- TEST 2: deck = 10 unique cards in order, returned out of order -------------
	printf("--------------------TEST 2: deck = 10 unique cards in order, returned out of order-----------------------\n");
	shuffleFail = 0;
	for(int i = 0; i < 10; i++){
		G.deck[thisPlayer][i] = i+5;
	}
	G.deckCount[thisPlayer] = 10;
	memcpy(&testG, &G, sizeof(struct gameState));
	shuffle(thisPlayer, &testG);
	assertTrue(&testG, &G, shuffleFail, thisPlayer);
	
	return 0;
}

int assertTrue(struct gameState* testG, struct gameState* G, int shuffleFail, int thisPlayer){

	if(shuffleFail < 0){
		printf("shuffle function fail - returned -1\n");
	}else{
		printf("Deck Check:\n");
		printf("Before shuffle: ");
		for(int i = 0; i < G->deckCount[thisPlayer]; i++){
			printf("%d ", G->deck[thisPlayer][i]);
		}
		printf("After shuffle: ");
		for(int i = 0; i < testG->deckCount[thisPlayer]; i++){
			printf("%d ", testG->deck[thisPlayer][i]);
		}
		printf("\n");
	}

	return 0;
}
