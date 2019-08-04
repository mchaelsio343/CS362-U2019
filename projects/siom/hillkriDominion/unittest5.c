/*
 * unittest5.c
 *
 
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest5: unittest5.c dominion.o rngs.o
 *      gcc -o unittest5 -g  unittest5.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "mine"

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
    int returnValue = 0;
    int expectedReturnValue = 0;
    struct gameState G, testG;
    int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
            sea_hag, tribute, smithy, council_room};

    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);

    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);


    // ----------- TEST 1: Discard card is not neither copper, silver, or gold. --------------
    printf("TEST 1: Next player: Discard card is not neither copper, silver, or gold.\n");

    // set hand card
    setHandCards(&G, thisPlayer, mine, estate, copper, copper, copper);

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    choice1 = 1;
    choice2 = silver;
    returnValue = mineEffect(&testG, thisPlayer, choice1, choice2, 0);

    expectedReturnValue = -1;
    printf("Return value = %d, expected = %d\n", returnValue, expectedReturnValue);
    printAssert(returnValue == expectedReturnValue);

    draw = 0;
    printf("current player (player[0]): hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + draw);
    printAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + draw);

    discarded = 0;
    printf("current player (player[0]): discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + discarded);
    printAssert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + discarded);


    // ----------- TEST 2: Exchanged treasure is less than the lower bound of kingdom card.  --------------
    printf("TEST 2: Exchanged treasure is less than the lower bound of kingdom card.\n");

    // set hand card
    setHandCards(&G, thisPlayer, mine, estate, copper, copper, copper);

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    choice1 = 1;
    choice2 = curse - 1;
    returnValue = mineEffect(&testG, thisPlayer, choice1, choice2, 0);

    expectedReturnValue = -1;
    printf("Return value = %d, expected = %d\n", returnValue, expectedReturnValue);
    printAssert(returnValue == expectedReturnValue);

    draw = 0;
    printf("current player (player[0]): hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + draw);
    printAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + draw);

    discarded = 0;
    printf("current player (player[0]): discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + discarded);
    printAssert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + discarded);


    // ----------- TEST 3: Exchanged treasure is greater than the uppper bound of kingdom card.  --------------
    printf("TEST 3: Exchanged treasure is greater than the uppper bound of kingdom card.\n");

    // set hand card
    setHandCards(&G, thisPlayer, mine, estate, copper, copper, copper);

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    choice1 = 1;
    choice2 = treasure_map + 1;
    returnValue = mineEffect(&testG, thisPlayer, choice1, choice2, 0);

    expectedReturnValue = -1;
    printf("Return value = %d, expected = %d\n", returnValue, expectedReturnValue);
    printAssert(returnValue == expectedReturnValue);

    draw = 0;
    printf("current player (player[0]): hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + draw);
    printAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + draw);

    discarded = 0;
    printf("current player (player[0]): discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + discarded);
    printAssert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + discarded);


    // ----------- TEST 4: Exchanged treasure's cost greater than the discarded treasure's cost plus + 3. --------------
    printf("TEST 4: Exchanged treasure's cost greater than the discarded treasure's cost plus + 3.\n");

    // set hand card
    setHandCards(&G, thisPlayer, mine, copper, copper, copper, copper);

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    choice1 = 1;
    choice2 = gold;
    returnValue = mineEffect(&testG, thisPlayer, choice1, choice2, 0);

    expectedReturnValue = -1;
    printf("Return value = %d, expected = %d\n", returnValue, expectedReturnValue);
    printAssert(returnValue == expectedReturnValue);

    draw = 0;
    printf("current player (player[0]): hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + draw);
    printAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + draw);

    discarded = 0;
    printf("current player (player[0]): discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + discarded);
    printAssert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + discarded);


   // ----------- TEST 5: Exchanged treasure's cost is equal to the discarded treasure's cost plus + 3. --------------
    printf("TEST 5: Exchanged treasure's cost is equal to the discarded treasure's cost plus + 3\n");

    // set hand card
    setHandCards(&G, thisPlayer, mine, silver, silver, silver, silver);

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    choice1 = 1;
    choice2 = gold;
    returnValue = mineEffect(&testG, thisPlayer, choice1, choice2, 0);

    expectedReturnValue = 0;
    printf("Return value = %d, expected = %d\n", returnValue, expectedReturnValue);
    printAssert(returnValue == expectedReturnValue);

    draw = 1;
    discarded = 2; //played card + discarded treasure
    printf("current player (player[0]): hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + draw - discarded);
    printAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + draw - discarded);

    printf("current player (player[0]): discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + discarded);
    printAssert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + discarded);


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