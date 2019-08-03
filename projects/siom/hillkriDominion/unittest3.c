#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "ambassador"

int assertTrue(struct gameState* testG, struct gameState* G, int cardChoiceIndex, int returnToSupply, int nextPlayer, int thisPlayer, int cardChoice, int handPos, int discarded, int gained, int ambassadorFail);

int main() {

	int cardChoice = 0;//hand position index for card to show and return to supply
	int returnToSupply = 2;//quantity of chosen card to return to supply
	int thisPlayer = 0;
	int nextPlayer = 1;
	int handPos = 1;//hand position index for ambassador
	int discarded = 1;//amount of cards discarded by current player (**not returned to the supply**), should remain 1
	int gained = 1;//amount of cards gained by other players, should remain 1;
	int cardChoiceIndex = 13;//enumerated value of the chosen card to show and return to supply (13 = smithy card)
	int seed = 1000;
	int numPlayers = 2;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	
	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

	// ----------- TEST 1: cardChoice = 0 = position in hand = 0 --------------
 	printf("TEST 1: cardChoice = 0 = position in hand = 0\n");

	// copy the game state to a test case
	G.hand[thisPlayer][0] = smithy;
	G.hand[thisPlayer][1] = ambassador;
	G.hand[thisPlayer][2] = smithy;
	G.hand[thisPlayer][3] = copper;
	G.hand[thisPlayer][4] = copper;
	
	G.supplyCount[cardChoiceIndex] = 10;

	memcpy(&testG, &G, sizeof(struct gameState));
	int ambassadorFail = ambassadorEffect(&testG, cardChoice, returnToSupply, thisPlayer, handPos);
	assertTrue(&testG, &G, cardChoiceIndex, returnToSupply, nextPlayer, thisPlayer, cardChoice, handPos, discarded, gained, ambassadorFail);

	return 0;
}

int assertTrue(struct gameState* testG, struct gameState* G, int cardChoiceIndex, int returnToSupply, int nextPlayer, int thisPlayer, int cardChoice, int handPos, int discarded, int gained, int ambassadorFail) {

	if(ambassadorFail == 0){
		printf("chosen card supply count = %d, expected = %d\n", testG->supplyCount[cardChoiceIndex], G->supplyCount[cardChoiceIndex] + returnToSupply);
		printf("current player discard count = %d, expected = %d\n", testG->discardCount[thisPlayer], G->discardCount[thisPlayer] + discarded);
		printf("other player discard count = %d, expected = %d\n", testG->discardCount[nextPlayer], G->discardCount[nextPlayer] + gained);
		printf("hand count = %d, expected = %d\n", testG->handCount[thisPlayer], G->handCount[thisPlayer] - discarded - returnToSupply);
		printf("top of other player's discard = %d, expected = %d\n", testG->discard[nextPlayer][testG->discardCount[nextPlayer] - 1], G->hand[thisPlayer][cardChoice]);
		printf("top of current player's discard = %d, expected = %d\n", testG->discard[thisPlayer][testG->discardCount[thisPlayer] - 1], G->hand[thisPlayer][handPos]);	
	}else{
		printf("***********ambassadorEffect fail*********");
	}
	return 0;
}
