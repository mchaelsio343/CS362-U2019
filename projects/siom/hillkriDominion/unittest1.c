#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "baron"

int assertTrue(struct gameState* testG, struct gameState* G, int choice, int newCards, int discarded, int xtraCoins, int bonusBuys, int thisPlayer);

int main() {

	int bonusBuys = 1;
	int newCards = 1;
	int discarded = 1;
	int xtraCoins = 0;
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
 	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

	// ----------- TEST 1: choice1 = 0 = no estate cards, receive a new one --------------
	printf("TEST 1: choice1 = 0 = no estate cards, receive a new one\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	G.hand[thisPlayer][0] = copper;
	G.hand[thisPlayer][1] = smithy;
	G.hand[thisPlayer][2] = copper;
	G.hand[thisPlayer][3] = mine;
	G.hand[thisPlayer][4] = baron;
		
	baronEffect(&testG, choice1, thisPlayer);
	assertTrue(&testG, &G, choice1, newCards, discarded, xtraCoins, bonusBuys, thisPlayer);

	// ----------- TEST 2: choice1 = 1 = discard estate, +4 coins --------------
 	printf("TEST 2: choice1 = 1 = discard estate, +4 coins\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 1;
	newCards = 0;
	discarded = 2;
	xtraCoins = 4;

	G.hand[thisPlayer][0] = copper;
	G.hand[thisPlayer][1] = estate;
	G.hand[thisPlayer][2] = copper;
	G.hand[thisPlayer][3] = mine;
	G.hand[thisPlayer][4] = baron;

	baronEffect(&testG, choice1, thisPlayer);
	assertTrue(&testG, &G, choice1, newCards, discarded, xtraCoins, bonusBuys, thisPlayer);

	// ------------- TEST 3: choice1 = 1, no estate in hand = recieve a new one -------------
	printf("TEST 3: choice1 = 1, no estate in hand = receive a new one\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	newCards = 1;
	discarded = 1;
	xtraCoins = 0;
	
	G.hand[thisPlayer][0] = copper;
	G.hand[thisPlayer][1] = smithy;
	G.hand[thisPlayer][2] = copper;
	G.hand[thisPlayer][3] = mine;
	G.hand[thisPlayer][4] = baron;
	
	baronEffect(&testG, choice1, thisPlayer);
	assertTrue(&testG, &G, choice1, newCards, discarded, xtraCoins, bonusBuys, thisPlayer);

	printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);
	return 0;
}


int assertTrue(struct gameState* testG, struct gameState* G, int choice, int newCards, int discarded, int xtraCoins, int bonusBuys, int thisPlayer){

	printf("hand count = %d, expected = %d\n", testG->handCount[thisPlayer], G->handCount[thisPlayer] + newCards - discarded);
	printf("deck count = %d, expected = %d\n", testG->deckCount[thisPlayer], G->deckCount[thisPlayer] - newCards + discarded);
	printf("coins = %d, expected = %d\n", testG->coins, G->coins + xtraCoins);
	printf("buys = %d, expected = %d\n", testG->numBuys, G->numBuys + bonusBuys);

	return 0;
}
