#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "minion"

int handCheck(struct gameState* testG, struct gameState* G, int numPlayers);
int assertTrue(struct gameState* testG, struct gameState* G, int choice, int discarded, int xtraCoins, int xtraActions, int thisPlayer);

int main() {

	int choice1 = 1;
	int thisPlayer = 0;
	int numPlayers = 2;
	int xtraActions = 1;
	int xtraCoins = 2;
	int handPos = 0;
	int discarded = 1;
	int seed = 1000;	
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

	// ----------- TEST 1: choice1 = 1 = +2 coins --------------
	printf("\nTEST 1: choice1 = 1 = +2 coins\n");
	
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	handCheck(&testG, &G, numPlayers);
	minionEffect(&testG, choice1, thisPlayer, handPos);
	assertTrue(&testG, &G, choice1, discarded, xtraCoins, xtraActions, thisPlayer);
	handCheck(&testG, &G, numPlayers);

	// ----------- TEST 2: choice1 = 2 = discard and draw a new hand -----------------
	printf("\nTEST 2: choice1 = 2 = discard and draw a new hand\n");

	choice1 = 2;	
	handCheck(&testG, &G, numPlayers);
	minionEffect(&testG, choice1, thisPlayer, handPos);
	assertTrue(&testG, &G, choice1, discarded, xtraCoins, xtraActions, thisPlayer);
	handCheck(&testG, &G, numPlayers);

	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);
	return 0;
}

int handCheck(struct gameState* testG, struct gameState* G, int numPlayers) {
	printf("-------------------------------------------------\n");
	printf("Test Game Hand Check:\n");
	for(int i = 0; i < numPlayers; i++){
		printf("Current hand for player %d:\n", i+1);
		for(int j = 0; j < testG->handCount[i]; j++){
			printf("%d, ", testG->hand[i][j]);
		}
		printf("\n");
	}

	printf("Control Game Hand Check:\n");
	for(int i = 0; i < numPlayers; i++){
		printf("Current hand for player %d:\n", i+1);
		for(int j = 0; j < G->handCount[i]; j++){
			printf("%d, ", G->hand[i][j]);
		}
		printf("\n");
	}
	printf("-------------------------------------------------\n");

	return 0;
}

int assertTrue(struct gameState* testG, struct gameState* G, int choice, int discarded, int xtraCoins, int xtraActions, int thisPlayer) {

	printf("hand count = %d, expected = %d\n", testG->handCount[thisPlayer], G->handCount[thisPlayer] - discarded);	
	printf("discard count = %d, expected = %d\n", testG->discardCount[thisPlayer], G->discardCount[thisPlayer] + discarded);
	printf("actions = %d, expected = %d\n", testG->numActions, G->numActions + xtraActions);
	printf("coins = %d, expected = %d\n", testG->coins, G->coins + xtraCoins);

	return 0;
}
