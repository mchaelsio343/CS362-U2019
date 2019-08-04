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

int assertTrue(int check, struct gameState* G, struct gameState* testG, int currentPlayer, int choice, int handPos);

int main(){

	for(int i = 0; i < NUM_TESTS; i++){
		int choice = floor(Random() * 2)+ 1;
		int numPlayers = floor(Random() * MAX_PLAYERS);
		int currentPlayer = floor(Random() * numPlayers);
		struct gameState G, testG;
		int handPos = 0;
		int seed = floor(Random() * 1000);//testing with relatively low numbers to find what is stalling the test
		int k[10] = {feast, tribute, steward, gardens, embargo,
				smithy, village, sea_hag, baron, great_hall};
		initializeGame(numPlayers, k, seed, &G);
		G.handCount[currentPlayer] = floor(Random() * 50);//testing with low numbers to find what is stalling the test
		for(int j = 0; j < G.handCount[currentPlayer]; j++){
			G.hand[currentPlayer][j] = floor(Random() * 27);
			if(G.hand[currentPlayer][j] == minion){
				handPos = j;
			}
		}
		int check;
		memcpy(&testG, &G, sizeof(struct gameState));
		check = minionEffect(&testG, choice, currentPlayer, handPos);
		assertTrue(check, &G, &testG, currentPlayer, choice, handPos);
	}
	printResults();

	return 0;
}

int assertTrue(int check, struct gameState* G, struct gameState* testG, int currentPlayer, int choice, int handPos){
	
	G->numActions++;
	discardCard(handPos, currentPlayer, G, 0);
	if (choice == 1) {		//+2 coins
		G->coins += 2;
	}else if (choice == 2){	//discard hand, redraw 4, other players with 5+ cards discard hand and draw 4
		while (numHandCards(G) > 0) {	//discard hand
			discardCard(handPos, currentPlayer, G, 0);
		}

		for (int i = 0; i < 4; i++) {	//draw 4 cards
			drawCard(currentPlayer, G);
		}

		for (int i = 0; i < G->numPlayers; i++) {
			if (i != currentPlayer) {
				if (G->handCount[i] > 4) {
					while (G->handCount[i] > 0) {	//discard hand
						discardCard(handPos, i, G, 0);
					}

					for (int j = 0; j < 4; j++) {	//draw 4
						drawCard(j, G);
					}
				}
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

	printf("\nminionEffect test results:\n");
	printf("minionEffect returned 0 (successful runthrough) %d times out of %d\n", runSuccess, NUM_TESTS);
	printf("minionEffect returned -1 (failed runthrough) %d times out of %d\n", runFail, NUM_TESTS);
	printf("test gameState struct same as control gameState struct %d times out of %d\n", testSuccess, NUM_TESTS);
	printf("test gameState struct different than control gameState struct %d times out of %d\n\n", testFail, NUM_TESTS);

	return 0;
}
