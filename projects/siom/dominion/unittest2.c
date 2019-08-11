/*
 * unittest2.c
 *
 
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest2: unittest2.c dominion.o rngs.o
 *      gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "steward"

int main() {
    int newCards = 0;
    int discarded = 1;
    int xtraAction = 0;
    int xtraCoins = 0;
    int shuffledCards = 0;
    int draw = 0;

    int i, j, m;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int remove1, remove2;
    int seed = 1000;
    int numPlayers = 4;
    int thisPlayer = 0;
    int handCount = 5;
    struct gameState G, testG;
    int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
            sea_hag, tribute, smithy, council_room};

    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);

    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);


    // ----------- TEST 1 --------------
    printf("TEST 1: The player only has 1 card to discard (excluding the steward card).\n");

    // set hand card
    setHandCards(&G, thisPlayer, steward, copper, -1, -1, -1);
    G.handCount[thisPlayer] = 2;

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // call cardEffect
    choice1 = 3;
    handpos = 0;
    bonus = 0;
    choice2 = 1;
    choice3 = 1;
    cardEffect(steward, choice1, choice2, choice3, &testG, handpos, bonus);

    // print choice
    printf("choice1 = %d\n", choice1);
    printf("choice2 = %d\n", choice2);
    printf("choice3 = %d\n", choice3);
    // print hand 
    for ( i = 0 ; i < G.handCount[thisPlayer] ; i++){
        printf("Player hand [%d] = %d \n", i , G.hand[thisPlayer][i]);  
    }

    // print hand count 
    printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], 0 );
    printAssert(testG.handCount[thisPlayer] == 0);

    printf("\n >>>>> Testing complete %s <<<<<\n\n", TESTCARD);

    return 0;
}

void printAssert(int flag){
    if(flag) printf("Passed\n-\n");
    else printf("Failed\n-\n");
}

void setHandCards(struct gameState *G, int player, int card1, int card2, int card3, int card4, int card5){
    G->hand[player][0]=card1;
    G->hand[player][1]=card2;
    G->hand[player][2]=card3;
    G->hand[player][3]=card4;
    G->hand[player][4]=card5;
}

void printHand(struct gameState *G, int player){
    int i;
    for(i = 0 ; i < 5; i++){
        printf("Card %d on hand = %d\n", i+1, G->hand[player][i]);
    }
}

int getSpecificHandCount(struct gameState *G, int player, int card){
    int count = 0;
    int i;
    for(i = 0 ; i < 5; i++){
        if(G->hand[player][i] == card) count++;
    }
    return card;
}