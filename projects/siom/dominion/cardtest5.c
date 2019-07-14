/*
 * cardtest5.c
 *
 
 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest5: cardtest5.c dominion.o rngs.o
 *      gcc -o cardtest5 -g  cardtest5.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "drawCard()"

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
    int players[4] = {0, 0, 0, 0};
    struct gameState G, testG;
    int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
            sea_hag, tribute, smithy, council_room};


    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

    // ----------- TEST 1: Player has 0 card in deck and have 5 cards in discard. --------------
    printf("TEST 1: Player has 0 card in deck and have 5 cards in discard.\n");

    // initialize game
    initializeGame(numPlayers, k, seed, &G);

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // move all the cards on deck to discard
    for ( i = 0 ; i < testG.deckCount[thisPlayer] ; i++){
        testG.discard[thisPlayer][testG.discardCount[thisPlayer]-1] = testG.deck[thisPlayer][i];
        testG.deck[thisPlayer][i] = -1;
        testG.discardCount[thisPlayer]++;
    }
    testG.deckCount[thisPlayer] = 0;

    drawCard(thisPlayer, &testG);

    printf("Deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], 4);
    printAssert(testG.deckCount[thisPlayer] == 4);

    printf("Discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], 0);
    printAssert(testG.discardCount[thisPlayer] == 0);


    // ----------- TEST 2: Player has 5 cards in deck. --------------
    printf("TEST 2: Player has 5 cards in deck.\n");

    // initialize game
    initializeGame(numPlayers, k, seed, &G);

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    drawCard(thisPlayer, &testG);

    printf("Deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - 1);
    printAssert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer] - 1);

    printf("Discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer]);
    printAssert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer]);


    // ----------- TEST 2: Player has 5 cards in deck. --------------
    printf("TEST 3: Player has no cards in neither deck nor dicard.\n");

    // initialize game
    initializeGame(numPlayers, k, seed, &G);

    // empty the deck
    for ( i = 0 ; i < G.deckCount[thisPlayer] ; i++){
        G.deck[thisPlayer][i] = -1;
    }
    G.deckCount[thisPlayer] = 0;

    // empty the discard
    for ( i = 0 ; i < G.discardCount[thisPlayer] ; i++){
        G.discard[thisPlayer][i] = -1;
    }
    G.discardCount[thisPlayer] = 0;    


    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));



    returnValue = drawCard(thisPlayer, &testG);

    expectedReturnValue = -1;
    printf("Return value = %d, expected = %d\n", returnValue, expectedReturnValue);
    printAssert(returnValue == expectedReturnValue);

    printf("Deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);
    printAssert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer]);

    printf("Discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer]);
    printAssert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer]);


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