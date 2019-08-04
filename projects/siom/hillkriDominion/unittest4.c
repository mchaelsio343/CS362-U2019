/*
 * unittest4.c
 *
 
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest4: unittest4.c dominion.o rngs.o
 *      gcc -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "tribute"

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
    int tributeRevealedCards[2] = {-1, -1};
    struct gameState G, testG;
    int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
            sea_hag, tribute, smithy, council_room};

    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);

    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);


    // ----------- TEST 1: Next player: discard count = 0 & deck count = 1. Reveal card = copper. --------------
    printf("TEST 1: Next player: discard count = 0 & deck count = 1. Reveal card = copper.\n");

    // set next player discard count & deck count
    G.discardCount[thisPlayer+1] = 0;
    G.discard[thisPlayer+1][0] = copper;
    G.deckCount[thisPlayer+1] = 1;

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    tributeEffect(&testG, thisPlayer, thisPlayer + 1, tributeRevealedCards);

    xtraCoins = 2;
    printf("current player (player[0]): coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
    printAssert(testG.coins == G.coins + xtraCoins);

    draw = 0;
    printf("current player (player[0]): hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + draw);
    printAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + draw);

    xtraAction = 0;
    printf("current player (player[0]): number of action = %d, expected = %d\n", testG.numActions, G.numActions + xtraAction);
    printAssert(testG.numActions == G.numActions + xtraAction);

    // ----------- TEST 2: Next player: discard count = 1 & deck count = 0. Reveal card = estate. --------------
    printf("TEST 2: Next player: discard count = 1 & deck count = 0. Reveal card = estate.\n");

    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);

    // set next player discard count & deck count
    G.deckCount[thisPlayer+1] = 1;
    G.deck[thisPlayer+1][0] = estate;
    G.discardCount[thisPlayer+1] = 0;

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    tributeEffect(&testG, thisPlayer, thisPlayer + 1, tributeRevealedCards);

    xtraCoins = 0;
    printf("current player (player[0]): coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
    printAssert(testG.coins == G.coins + xtraCoins);

    draw = 2;
    printf("current player (player[0]): hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + draw);
    printAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + draw);

    xtraAction = 0;
    printf("current player (player[0]): number of action = %d, expected = %d\n", testG.numActions, G.numActions + xtraAction);
    printAssert(testG.numActions == G.numActions + xtraAction);
    

    // ----------- TEST 3: Next player: discard count = 2 & deck count = 0. 1st reveal card = estate. 2nd reveal card = copper. --------------
    printf("TEST 3: Next player: discard count = 2 & deck count = 0. 1st reveal card = estate. 2nd reveal card = copper.\n");

    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);

    // set next player discard count & deck count
    G.deckCount[thisPlayer+1] = 0;
    G.discardCount[thisPlayer+1] = 2;
    G.discard[thisPlayer+1][0] = estate;
    G.discard[thisPlayer+1][1] = copper;

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    tributeEffect(&testG, thisPlayer, thisPlayer + 1, tributeRevealedCards);

    xtraCoins = 2;
    printf("current player (player[0]): coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
    printAssert(testG.coins == G.coins + xtraCoins);

    draw = 2;
    printf("current player (player[0]): hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + draw);
    printAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + draw);

    xtraAction = 0;
    printf("current player (player[0]): number of action = %d, expected = %d\n", testG.numActions, G.numActions + xtraAction);
    printAssert(testG.numActions == G.numActions + xtraAction);


    // ----------- TEST 4: Two revealed cards are the same. --------------
    printf("TEST 4: Two revealed cards are the same.\n");

    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);

    // set next player discard count & deck count
    G.deckCount[thisPlayer+1] = 2;
    G.deck[thisPlayer+1][0] = copper;
    G.deck[thisPlayer+1][1] = copper;

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    tributeEffect(&testG, thisPlayer, thisPlayer + 1, tributeRevealedCards);

    xtraCoins = 2;
    printf("current player (player[0]): coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
    printAssert(testG.coins == G.coins + xtraCoins*2);

    draw = 0;
    printf("current player (player[0]): hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + draw);
    printAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + draw);

    xtraAction = 0;
    printf("current player (player[0]): number of action = %d, expected = %d\n", testG.numActions, G.numActions + xtraAction);
    printAssert(testG.numActions == G.numActions + xtraAction);


    // ----------- TEST 5: Next player has no cards on neither deck or discard --------------
    printf("TEST 5: Next player has no cards on neither deck or discard.\n");

    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);

    // reset tributeRevealedCards
    tributeRevealedCards[0] = -1;
    tributeRevealedCards[1] = -1;

    // set next player discard count & deck count
    G.deckCount[thisPlayer+1] = 0;
    G.discardCount[thisPlayer+1] = 0;

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    tributeEffect(&testG, thisPlayer, thisPlayer + 1, tributeRevealedCards);

    xtraCoins = 0;
    printf("current player (player[0]): coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
    printAssert(testG.coins == G.coins + xtraCoins);

    draw = 0;
    printf("current player (player[0]): hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + draw);
    printAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + draw);

    xtraAction = 0;
    printf("current player (player[0]): number of action = %d, expected = %d\n", testG.numActions, G.numActions + xtraAction);
    printAssert(testG.numActions == G.numActions + xtraAction);


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