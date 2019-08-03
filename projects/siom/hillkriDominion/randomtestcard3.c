#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <math.h>

#define REASONABLE_VALUE 25
#define NUM_TESTS 300


int runSuccess;
int runFail;
int testSuccess;
int testFail;

int printResults();

int assertTrue(int check, struct gameState* G, struct gameState* testG, int currentPlayer, int nextPlayer, int* tributeRevealCards);

int main(){

	for(int i = 0; i < NUM_TESTS; i++){
		int tributeRevealCards[2];
		int testTributeRevealCards[2];
		int numPlayers = floor(Random() * MAX_PLAYERS);
		int currentPlayer = floor(Random() * numPlayers);
		int nextPlayer;
		if(currentPlayer > 2){
			nextPlayer = 0;
		}else{
			nextPlayer = currentPlayer+1;
		}
		struct gameState G, testG;
		int seed = floor(Random() * 1000);//testing with relatively low numbers to find what is stalling the test
		int k[10] = {feast, tribute, steward, gardens, embargo,
				smithy, village, sea_hag, baron, great_hall};
		initializeGame(numPlayers, k, seed, &G);
		G.handCount[currentPlayer] = floor(Random() * REASONABLE_VALUE);//testing with low numbers to find what is stalling the test
		for(int j = 0; j < G.handCount[currentPlayer]; j++){
			G.hand[currentPlayer][j] = floor(Random() * 27);
		}
		G.deckCount[currentPlayer] = floor(Random() * REASONABLE_VALUE);
		for(int j = 0; j < G.deckCount[currentPlayer]; j++){
			G.deck[currentPlayer][j] = floor(Random() * 27);
		}
		G.discardCount[currentPlayer] = floor(Random() * REASONABLE_VALUE);
		for(int j = 0; j < G.discardCount[currentPlayer]; j++){
			G.discard[currentPlayer][j] = floor(Random() * 27);
		}
		G.deckCount[nextPlayer] = floor(Random() * REASONABLE_VALUE);
		for(int j = 0; j < G.deckCount[nextPlayer]; j++){
			G.deck[nextPlayer][j] = floor(Random() * 27);
		}
		G.discardCount[nextPlayer] = floor(Random() * REASONABLE_VALUE);
		for(int j = 0; j < G.discardCount[nextPlayer]; j++){
			G.discard[nextPlayer][j] = floor(Random() * 27);
		}
		G.playedCardCount = floor(Random() * REASONABLE_VALUE);
		for(int j = 0; j < G.playedCardCount; j++){
			G.playedCards[j] = floor(Random() * 27);
		}
		int check;
		memcpy(&testG, &G, sizeof(struct gameState));
		check = tributeEffect(&testG, currentPlayer, nextPlayer, testTributeRevealCards);
		assertTrue(check, &G, &testG, currentPlayer, nextPlayer, tributeRevealCards);
	}
	printResults();

	return 0;
}

int assertTrue(int check, struct gameState* G, struct gameState* testG, int currentPlayer, int nextPlayer, int* tributeRevealCards){
	
	if (G->deckCount[nextPlayer] == 0) {
		for (int i = 0; i < G->discardCount[nextPlayer]; i++) {
			G->deck[nextPlayer][i] = G->discard[nextPlayer][i];//Move to deck
			G->deckCount[nextPlayer]++;
			G->discard[nextPlayer][i] = -1;
			G->discardCount[nextPlayer]--;
		}

		shuffle(nextPlayer, G);//Shuffle the deck
	}

	for (int i = 0; i < 2; i++) {	//reveal the two cards from the next player's deck
		tributeRevealCards[i] = G->deck[nextPlayer][G->deckCount[nextPlayer]-1];
		G->deck[nextPlayer][G->deckCount[nextPlayer]--] = -1;
		G->deckCount[nextPlayer]--;
	}


	if (tributeRevealCards[0] == tributeRevealCards[1]) {//If we have a duplicate card, just drop one 
		G->playedCards[G->playedCardCount] = tributeRevealCards[1];
		G->playedCardCount++;
		tributeRevealCards[1] = -1;
	}

	for (int i = 0; i < 2; i++) {
		if (tributeRevealCards[i] == copper || tributeRevealCards[i] == silver || tributeRevealCards[i] == gold) {//Treasure cards
			G->coins += 2;
		}
		else if (tributeRevealCards[i] == estate || tributeRevealCards[i] == duchy || tributeRevealCards[i] == province || tributeRevealCards[i] == gardens || tributeRevealCards[i] == great_hall) {//Victory Card Found
			for (int j = 0; j < 2; j++) {
				drawCard(currentPlayer, G);
			}
		}
		else {//Action Card
			G->numActions += 2;
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
	
	printf("\ntributeEffect test results:\n");
	printf("tributeEffect returned 0 (successful runthrough) %d times out of %d\n", runSuccess, NUM_TESTS);
	printf("tributeEffect returned -1 (failed runthrough) %d times out of %d\n", runFail, NUM_TESTS);
	printf("test gameState struct same as control gameState struct %d times out of %d\n", testSuccess, NUM_TESTS);
	printf("test gameState struct different than control gameState struct %d times out of %d\n\n", testFail, NUM_TESTS);
	
	return 0;
}
