#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

int assertTrue(struct gameState* testG, struct gameState* G, int draw, int thisPlayer, int hasDeck);

int main() {

	int draw = 1;
	int thisPlayer = 0;
	int hasDeck = 1;
	int seed = 1000;
	int numPlayers = 2;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	
	printf("----------------- Testing Function: drawCard ----------------\n");
	
	// ----------- TEST 1: hasDeck = 1 = no shuffling, just pulling card from deck --------------
	printf("TEST 1: hasDeck = 1 = true = no shuffling, just pulling card from deck\n");
	G.deckCount[thisPlayer] = 0;
	for(int i = 0; i < 10; i++){
		G.deck[thisPlayer][i] = i+5;
		G.deckCount[thisPlayer]++;
	}
	for(int i = 0; i < G.handCount[thisPlayer]; i++){
		G.hand[thisPlayer][i] = -1;
	}
	G.handCount[thisPlayer] = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	drawCard(thisPlayer, &testG);
	assertTrue(&testG, &G, draw, thisPlayer, hasDeck);
	
	// ----------- TEST 2: hasDeck = 0 = move cards from discard to deck before drawing  --------------
	printf("TEST 2: hasDeck = 0 = false = move cards from discard to deck before drawing\n");
	for(int i = 0; i < G.deckCount[thisPlayer]; i++){
		G.deck[thisPlayer][i] = -1;
	}
	G.deckCount[thisPlayer] = 0;
	for(int i = 0; i < G.handCount[thisPlayer]; i++){
		G.hand[thisPlayer][i] = -1;
	}
	G.handCount[thisPlayer] = 0;
	for(int i = 0; i < 10; i++){
		G.discard[thisPlayer][i] = i+5;
		G.discardCount[thisPlayer]++;
	}
	hasDeck = 0;
	memcpy(&testG, &G, sizeof(struct gameState));
	drawCard(thisPlayer, &testG);
	assertTrue(&testG, &G, draw, thisPlayer, hasDeck);

	return 0;
}

int assertTrue(struct gameState* testG, struct gameState* G, int draw, int thisPlayer, int hasDeck){

	if(hasDeck){
		printf("drawn card = %d, expected card = %d\n", testG->hand[thisPlayer][testG->handCount[thisPlayer]-1], G->deck[thisPlayer][G->deckCount[thisPlayer]-1]);
		printf("deck count = %d, expected value = %d\n", testG->deckCount[thisPlayer], G->deckCount[thisPlayer] - draw);
	}else{
		printf("discard count = %d, expected value = %d\n", testG->discardCount[thisPlayer], G->deckCount[thisPlayer]);
		printf("deck count = %d, expected value = %d\n", testG->deckCount[thisPlayer], G->discardCount[thisPlayer] - draw);
	}
	printf("hand count = %d, expected value = %d\n", testG->handCount[thisPlayer], G->handCount[thisPlayer] + draw);	

	return 0;
}
