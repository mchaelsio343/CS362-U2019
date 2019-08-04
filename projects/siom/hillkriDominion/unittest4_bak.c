#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "tribute"

int assertTrue(struct gameState* testG, struct gameState* G, int xtraCoins, int xtraCards, int xtraActions, int revealed, int thisPlayer, int nextPlayer);

int main() {

	int xtraCoins = 2;
	int xtraCards = 2;
	int xtraActions = 0;
	int thisPlayer = 0;
	int nextPlayer = 1;
	int revealed = 2;
	int revealedCards[2];

	int seed = 1000;
	int numPlayers = 2;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
 	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

	// ----------- TEST 1: revealed cards = copper and estate  = +2 coins, +2 cards in hand--------------
	printf("TEST 1: revealed cards = copper and estate = +2 coins, +2 cards in hand\n");
	
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.deck[nextPlayer][4] = copper;
	testG.deck[nextPlayer][3] = estate;
	tributeEffect(&testG, thisPlayer, nextPlayer, revealedCards);
	assertTrue(&testG, &G, xtraCoins, xtraCards, xtraActions, revealed, thisPlayer, nextPlayer);

	// ----------- TEST 2: revealed cards = copper and smithy = +2 coins, +2 actions--------------
	printf("TEST 2: revealed cards = copper and smithy = +2 coins, +2 actions\n");
	
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.deck[nextPlayer][4] = copper;
	testG.deck[nextPlayer][3] = smithy;
	xtraCards = 0;
	xtraActions = 2;
	tributeEffect(&testG, thisPlayer, nextPlayer, revealedCards);
	assertTrue(&testG, &G, xtraCoins, xtraCards, xtraActions, revealed, thisPlayer, nextPlayer);

	return 0;
}
int assertTrue(struct gameState* testG, struct gameState* G, int xtraCoins, int xtraCards, int xtraActions, int revealed, int thisPlayer, int nextPlayer) {

	printf("coins = %d, expected = %d\n", testG->coins, G->coins + xtraCoins);
	printf("hand count = %d, expected = %d\n", testG->handCount[thisPlayer], G->handCount[thisPlayer] + xtraCards);
	printf("actions = %d, expected = %d\n", testG->numActions, G->numActions + xtraActions);
	printf("deck count for player 2 = %d, expected = %d\n", testG->deckCount[nextPlayer], G->deckCount[nextPlayer] - revealed);

	return 0;
}
