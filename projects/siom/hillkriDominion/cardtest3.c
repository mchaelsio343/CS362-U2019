#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

int assertTrue(struct gameState* testG, struct gameState* G, int nextPlayer);

int main() {

	int seed = 1000;
	int numPlayers = 2;
	int nextPlayer = 1;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	printf("----------------- Testing Function: endTurn() ----------------\n");
	//---------------------- TEST 1: endTurn occurs on player 1's turn --------------------
	printf("--------------------TEST 1: endTurn occurs on player 1's turn-----------------------\n");
	memcpy(&testG, &G, sizeof(struct gameState));
	endTurn(&testG);
	assertTrue(&testG, &G, nextPlayer);
	return 0;
}

int assertTrue(struct gameState* testG, struct gameState* G, int nextPlayer){
	
	printf("Started on Player %d's turn, is now Player %d's turn\n", G->whoseTurn, testG->whoseTurn);
	printf("Next player's hand before endTurn(): ");
	if(G->handCount[nextPlayer] < 1){
		printf("No cards in their hand\n");
	}else{
		for(int i = 0; i < G->handCount[nextPlayer]; i++){
			printf("%d ", G->hand[nextPlayer][i]);
		}
		printf("\n");
	}
	printf("Same player's hand after endTurn(): ");
	for(int i = 0; i < testG->handCount[testG->whoseTurn]; i++){
		printf("%d ", testG->hand[testG->whoseTurn][i]);
	}
	printf("\n");
	printf("outpost played = %d, expected value = 0\n", testG->outpostPlayed);	
	printf("phase = %d, expected value = 0\n", testG->phase);
	printf("number of actions = %d, expected value = 1\n", testG->numActions);
	printf("number of buys = %d, expected value = 1\n", testG->numBuys);
	printf("played card count = %d, expected value = 0\n", testG->playedCardCount);

	return 0;
}
