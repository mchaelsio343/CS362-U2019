#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>

int assertTrue(struct gameState* testG, int* k, int curseCount, int victoryCount, int copperCount, int silverCount, int goldCount, int hallOrGardensCount, int kingdomCardCount, int thisPlayer);

int main() {

	int numPlayers = 2;//start with 2 players
	int curseCount = 10;//start with value for 2 players
	int victoryCount = 8;//start with value for 2 players
	int copperCount = 46;//start with value for 2 players; 60-(2*7)
	int silverCount = 40;
	int goldCount = 30;
	int hallOrGardensCount = 8;//start with value for 2 players
	int kingdomCardCount = 10;
	int seed = 1000;
	int thisPlayer = 0;
	struct gameState test1;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, great_hall};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &test1);
	
	printf("----------------- Testing initializeGame function ----------------\n");
	
	// ----------- TEST 1: 2-player game--------------
	printf("TEST 1: 2-player game\n");
	assertTrue(&test1, k, curseCount, victoryCount, copperCount, silverCount, goldCount, hallOrGardensCount, kingdomCardCount, thisPlayer);

	// ----------- TEST 2: 3-player game -------------
	
	printf("TEST 2: 3-player game\n");
	numPlayers = 3;
	curseCount = 20;
	victoryCount = 12;
	hallOrGardensCount = 12;
	struct gameState test2;
	initializeGame(numPlayers, k, seed, &test2);
	assertTrue(&test2, k, curseCount, victoryCount, copperCount, silverCount, goldCount, hallOrGardensCount, kingdomCardCount, thisPlayer);

	// ---------- TEST 3: 4-player game -------------
	printf("TEST 3: 4-player game\n");
	numPlayers = 4;
	curseCount = 30;
	struct gameState test3;
	initializeGame(numPlayers, k, seed, &test3);
	assertTrue(&test3, k, curseCount, victoryCount, copperCount, silverCount, goldCount, hallOrGardensCount, kingdomCardCount, thisPlayer);
	return 0;
}

int assertTrue(struct gameState* testG, int* k, int curseCount, int victoryCount, int copperCount, int silverCount, int goldCount, int hallOrGardensCount, int kingdomCardCount, int thisPlayer) {

	printf("------------------------------------------------------\n");
	printf("curse supply count = %d, expected = %d\n", testG->supplyCount[curse], curseCount);
	printf("supply count for estate = %d, duchy = %d, province = %d; expected for each = %d\n", testG->supplyCount[estate], testG->supplyCount[duchy], testG->supplyCount[province], victoryCount); 
	printf("copper supply count = %d, expected = %d\n", testG->supplyCount[copper], copperCount);
	printf("silver supply count = %d, expected = %d\n", testG->supplyCount[silver], silverCount);
	printf("gold supply count = %d, expected = %d\n", testG->supplyCount[gold], goldCount);
	for(int i = 7; i < 28; i++){//7 = adventurer card; 28 = treasure_map card + 1
		for(int j = 0; j < 10; j++){
			if(k[j] == i){
				if(k[j] == 17 || k[j] == 11){//17 = great_hall card; 11 = gardens card
					printf("supply count for card %d = %d, expected = %d\n", k[j], testG->supplyCount[i], hallOrGardensCount);
				}else{
					printf("supply count for card %d = %d, expected = %d\n", k[j], testG->supplyCount[i], kingdomCardCount);
				}break;
			}
		}
	}
	printf("Player 1's deck:\n");
	int estateDeckCheck = 0;
	int copperDeckCheck = 0;
	int garbageDeckCheck = 0;
	for(int i = 0; i < 10; i++){
		if(testG->deck[thisPlayer][i] == 1){//1 = estate card
			estateDeckCheck++;
		}else if(testG->deck[thisPlayer][i] == 4){//4 = copper card
			copperDeckCheck++;
		}else{//make sure there aren't any cards other than copper and estate in the initial hand
			garbageDeckCheck++;
		}
	}
	printf("estate cards in deck = %d, expected = 3\n", estateDeckCheck);
	printf("copper cards in deck = %d, expected = 7\n", copperDeckCheck);
	printf("other cards in deck = %d, expected = 0\n", garbageDeckCheck);
	printf("total cards in deck = %d, expected = 10\n", testG->deckCount[thisPlayer]);
	printf("-----------------------------------------------------------------\n");

	printf("deck count = %d, expected = 5\n", testG->handCount[thisPlayer]);
	printf("outpost played count = %d, expected = 0\n", testG->outpostPlayed);
	printf("phase number = %d, expected = 0\n", testG->phase);
	printf("number of actions = %d, expected = 1\n", testG->numActions);
	printf("number of buys = %d, expected = 1\n", testG->numBuys);
	printf("played card count = %d, expected 0\n", testG->playedCardCount);
	printf("currently player %d's turn, should be player 1's turn\n", testG->whoseTurn + 1);

	return 0;
}
