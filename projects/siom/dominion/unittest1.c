/*
 * unittest1.c
 *
 
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest1: unittest1.c dominion.o rngs.o
 *      gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "baron"

int main() {
    int newCards = 0;
    int discarded = 1;
    int xtraCoins = 0;
    int shuffledCards = 0;

    int i, j, m;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int remove1, remove2;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
    int handCount = 5;
    struct gameState G, testG;
    int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
            sea_hag, tribute, smithy, council_room};

    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);

    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

    // ----------- TEST 1: choice1 = 1. Discard an estate and gain 4 coins. Player has at least one estate. --------------
    printf("TEST 1: choice1 = 1. Discard an estate and gain 4 coins. Player has at least one estate.\n");

    // set all the hand cards to Estate
    setHandCards(&G, thisPlayer, estate, estate, estate, estate, estate);

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));
    choice1 = 1;
    baronEffect(&testG, choice1, thisPlayer);

    xtraCoins = 4; // from the effect
    discarded = 2; // played card + estate
    printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] - discarded);
    printAssert(testG.handCount[thisPlayer] == (G.handCount[thisPlayer] - discarded));
    
    printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);
    printAssert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer]);
    
    printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
    printAssert(testG.coins == G.coins + xtraCoins);

    printf("dicard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + discarded);
    printAssert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + discarded);


    // ----------- TEST 2: choice1 = 0. Gain an estate. --------------
    printf("TEST 2: choice1 = 0. Gain an estate.\n");

    // set hand cards with one estate plus others card rather than estate
    setHandCards(&G, thisPlayer, estate, copper, copper, copper, copper);

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));
    choice1 = 0;
    baronEffect(&testG, choice1, thisPlayer);

    newCards = 1;
    discarded = 1; // played card
    printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] - discarded);
    printAssert(testG.handCount[thisPlayer] == (G.handCount[thisPlayer] - discarded));
    
    printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);
    printAssert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer]);
    
    printf("coins = %d, expected = %d\n", testG.coins, G.coins);
    printAssert(testG.coins == G.coins);

    printf("dicard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + discarded + newCards);
    printAssert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + discarded + newCards);


    // ----------- TEST 3: choice1 = 3 = trash two cards --------------

    printf("TEST 3: choice1 = 0. Discard an estate and gain 4 coins. But Player has no estate.\n");

    // set hand cards with one estate plus others card rather than estate
    setHandCards(&G, thisPlayer, copper, copper, copper, copper, copper);

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));
    choice1 = 1;
    baronEffect(&testG, choice1, thisPlayer);

    newCards = 1;
    discarded = 1; // played card
    printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] - discarded);
    printAssert(testG.handCount[thisPlayer] == (G.handCount[thisPlayer] - discarded));
    
    printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);
    printAssert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer]);
    
    printf("coins = %d, expected = %d\n", testG.coins, G.coins);
    printAssert(testG.coins == G.coins);

    printf("dicard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + discarded + newCards);
    printAssert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + discarded + newCards);


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