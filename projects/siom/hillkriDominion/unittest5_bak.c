#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "mine"

int assertTrue(struct gameState* testG, struct gameState* G, int xtraCard, int thisPlayer, int played, int upgradeCard, int trashed, int trashCard, int mineFail);

int main() {

	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;
	int xtraCard = 1;//quantity of cards gained
	int played = 1;//quantity of cards played
	int trashed = 1;//quantity of cards trashed
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
	int trashCard = 0;//hand position of the card to trash
	int upgradeCard = 5;//enumerated value for silver
	int handPos = 1;//hand position of the mine card
	int mineFail = 0;//test for function fail (where it returns -1)
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	
	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
	
	// ----------- TEST 1: choice1 = 0 = trash copper; choice2 = 5 = get silver --------------
	printf("TEST 1: choice1 = 0 = trash card at hand position 0 (copper); upgradeCard = 5 = get silver\n");
	
	G.hand[0][0] = copper;
	G.hand[0][1] = mine;
	G.hand[0][2] = tribute;
	G.hand[0][3] = duchy;
	G.hand[0][4] = smithy;
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	mineFail = mineEffect(&testG, thisPlayer, trashCard, upgradeCard, handPos);
	assertTrue(&testG, &G, xtraCard, thisPlayer, played, upgradeCard, trashed, trashCard, mineFail);

	// ----------- TEST 2: trashCard = 0 = trash silver; upgradeCard = 6 = get gold --------------
	printf("TEST 2: trashCard = 0 = trash card at hand position 0 (silver); upgradeCard = 6 = get gold\n");

	G.hand[0][0] = silver;
	G.hand[0][1] = mine;
	G.hand[0][2] = tribute;
	G.hand[0][3] = duchy;
	G.hand[0][4] = smithy;
	upgradeCard = 6;
	mineFail = 0;///reset boolean
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	mineFail = mineEffect(&testG, thisPlayer, trashCard, upgradeCard, handPos);
	assertTrue(&testG, &G, xtraCard, thisPlayer, played, upgradeCard, trashed, trashCard, mineFail);

	// ----------- TEST 3: trashCard = 0 = trash gold; upgradeCard = 6 = get gold --------------
	printf("TEST 3: trashCard = 0 = trash card at hand position 0 (gold); upgradeCard = 6 = get gold\n");
	
	G.hand[0][0] = gold;
	G.hand[0][1] = mine;
	G.hand[0][2] = tribute;
	G.hand[0][3] = duchy;
	G.hand[0][4] = smithy;

	mineFail = 0;///reset boolean
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	mineFail = mineEffect(&testG, thisPlayer, trashCard, upgradeCard, handPos);
	assertTrue(&testG, &G, xtraCard, thisPlayer, played, upgradeCard, trashed, trashCard, mineFail);

	// ----------- TEST 4: trashCard = 3 = try trashing a non-treasure card; upgradeCard = 6 = get gold --------------
	printf("TEST 4: trashCard = 3 = try trashing a non-treasure card; upgradeCard = 6 = get gold\n");
	
	G.hand[0][0] = gold;
	G.hand[0][1] = mine;
	G.hand[0][2] = tribute;
	G.hand[0][3] = duchy;
	G.hand[0][4] = smithy;

	mineFail = 0;///reset boolean
	memcpy(&testG, &G, sizeof(struct gameState));
	mineFail = mineEffect(&testG, thisPlayer, trashCard, upgradeCard, handPos);
	assertTrue(&testG, &G, xtraCard, thisPlayer, played, upgradeCard, trashed, trashCard, mineFail);
	
	// ----------- TEST 5: trashCard = 0 = gold; upgradeCard = 8 = try upgrading to non-treasure card --------------
	printf("TEST 5: trashCard = 0 = gold; upgradeCard = 8 = try upgrading to non-treasure card\n");
	
	upgradeCard = 8;

	mineFail = 0;///reset boolean
	memcpy(&testG, &G, sizeof(struct gameState));
	mineFail = mineEffect(&testG, thisPlayer, trashCard, upgradeCard, handPos);
	assertTrue(&testG, &G, xtraCard, thisPlayer, played, upgradeCard, trashed, trashCard, mineFail);

	// ----------- TEST 6: trashCard = 0 = copper; upgradeCard = 6 = get gold --------------
	printf("TEST 6: trashCard = 0 = copper; upgradeCard = 6 = get gold\n");
	
	G.hand[0][0] = copper;

	upgradeCard = 6;
	mineFail = 0;///reset boolean

	memcpy(&testG, &G, sizeof(struct gameState));
	mineFail = mineEffect(&testG, thisPlayer, trashCard, upgradeCard, handPos);
	assertTrue(&testG, &G, xtraCard, thisPlayer, played, upgradeCard, trashed, trashCard, mineFail);

	return 0;
}

int assertTrue(struct gameState* testG, struct gameState* G, int xtraCard, int thisPlayer, int played, int upgradeCard, int trashed, int trashCard, int mineFail) {

	if(mineFail == 0){
		printf("new card in hand = %d, expected value = %d\n", testG->hand[thisPlayer][testG->handCount[thisPlayer]-1], upgradeCard);
		printf("hand count = %d, expected value = %d\n", testG->handCount[thisPlayer], G->handCount[thisPlayer] - played - trashed + xtraCard);
		printf("supply count = %d, expected value = %d\n", testG->supplyCount[upgradeCard], G->supplyCount[upgradeCard] - xtraCard);
		printf("top card in played card pile = %d, expected card = %d\n", testG->playedCards[testG->playedCardCount - 1], G->hand[thisPlayer][trashCard]);
		printf("played card count = %d, expected value = %d\n", testG->playedCardCount, G->playedCardCount + played);	
	}else{
		printf("mineEffect function failed\n");
	}

	return 0;
}
