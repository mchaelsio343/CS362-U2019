#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <math.h>

#define NUM_TESTS 300
int runSuccess;
int runFail;
int testSuccess;
int testFail;

int printResults();

int assertTrue(int check, struct gameState* G, struct gameState* testG, int currentPlayer, int discardEstate);

int main(){

	for(int i = 0; i < NUM_TESTS; i++){
		int discardEstate = floor(Random() * 2);
		int numPlayers = floor(Random() * MAX_PLAYERS);
		int currentPlayer = floor(Random() * numPlayers);
		struct gameState G, testG;
		int seed = floor(Random() * 1000);//testing with relatively low numbers to find what is stalling the test
		int k[10] = {feast, tribute, steward, gardens, embargo,
				smithy, village, sea_hag, baron, great_hall};
		initializeGame(numPlayers, k, seed, &G);
		G.handCount[currentPlayer] = floor(Random() * 50);//testing with low numbers to find what is stalling the test
		for(int j = 0; j < G.handCount[currentPlayer]; j++){
			G.hand[currentPlayer][j] = floor(Random() * 27);
		}
		int check;
		memcpy(&testG, &G, sizeof(struct gameState));
		check = baronEffect(&testG, discardEstate, currentPlayer);
		assertTrue(check, &G, &testG, currentPlayer, discardEstate);
	}
	printResults();
	return 0;
}

int assertTrue(int check, struct gameState* G, struct gameState* testG, int currentPlayer, int discardEstate){
	
	G->numBuys++;
	if(discardEstate){
		int cardIterator = 0;//Iterator for hand!
		int card_not_discarded = 1;//Flag for discard set!
		while (card_not_discarded) {
			int currentCard = G->hand[currentPlayer][cardIterator];
			if (currentCard == estate) {//Found an estate card!
				G->coins += 4;//Add 4 coins to the amount of coins
				discardCard(currentCard, currentPlayer, G, 1);//trash the estate card
				card_not_discarded = 0;//Exit the loop
			}
			else if (cardIterator > G->handCount[currentPlayer]) {
				int gainEstate = gainCard(estate, G, 0, currentPlayer);
				card_not_discarded = 0;//Exit the loop
			}
			else {
				cardIterator++;//Next card
			}
		}

	}

	if(check == 0){
		runSuccess++;
		if(memcmp(&testG, &G, sizeof(struct gameState)) == 0){
			testSuccess++;
		}else{
			testFail++;
		}
	}else{
		runFail++;
	}
	return 0;
}

int printResults(){
	
	printf("\nbaronEffect test results:\n");
	printf("baronEffect returned 0 (successful runthrough) %d times out of %d\n", runSuccess, NUM_TESTS);
	printf("baronEffect returned -1 (failed runthrough) %d times out of %d\n", runFail, NUM_TESTS);
	printf("test gameState struct same as control gameState struct %d times out of %d\n", testSuccess, NUM_TESTS);
	printf("test gameState struct different than control gameState struct %d times out of %d\n\n", testFail, NUM_TESTS);
	return 0;
}
