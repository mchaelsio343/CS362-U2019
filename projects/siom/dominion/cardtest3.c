/*
 * cardtest3.c
 *
 
 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest3: cardtest3.c dominion.o rngs.o
 *      gcc -o cardtest3 -g  cardtest3.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "endTurn()"

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
    int numPlayers = 2;
    int maxPlayers = 4;
    int thisPlayer = 0;
    int handCount = 5;
    int returnValue = 0;
    int expectedReturnValue = 0;
    int expectedCount = 0;
    struct gameState G, testG;
    int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
            sea_hag, tribute, smithy, council_room};


    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

    // ----------- TEST 1: Current player and next player. --------------
    printf("TEST 1: Current player and next player.\n");

    // initialize game
    initializeGame(numPlayers, k, seed, &G);

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    endTurn(&testG);

    printf("# current player (player[0]) # \n");
    printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], 0);
    printAssert(testG.handCount[thisPlayer] == 0);
    
    printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);
    printAssert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer]);

    printf("dicard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + G.handCount[thisPlayer]);
    printAssert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + G.handCount[thisPlayer]);


    printf("# next player (player[1]) # \n");
    printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer+1], 5);
    printAssert(testG.handCount[thisPlayer+1] == 5);
    
    printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer+1], G.deckCount[thisPlayer+1] - 5);
    printAssert(testG.deckCount[thisPlayer+1] == G.deckCount[thisPlayer+1] - 5);

    printf("dicard count = %d, expected = %d\n", testG.discardCount[thisPlayer+1], G.discardCount[thisPlayer+1]);
    printAssert(testG.discardCount[thisPlayer+1] == G.discardCount[thisPlayer+1]);


    // ----------- TEST 2: Current player is the last one. --------------
    printf("TEST 2: Current player is the last one.\n");

    // initialize game
    numPlayers = 4;
    initializeGame(numPlayers, k, seed, &G);

    // set the current player be the last player 
    testG.whoseTurn = numPlayers - 1;

    endTurn(&testG);

    printf("whoseTurn = %d, expected = %d\n", testG.whoseTurn, 0);
    printAssert(testG.whoseTurn == 0);


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

int getSpecificCountInDeck(struct gameState *G, int player, int card){
    int count = 0;
    int i;
    for(i = 0 ; i < G->deckCount[player]; i++){
        if(G->deck[player][i] == card) count++;
    }
    return count;
}