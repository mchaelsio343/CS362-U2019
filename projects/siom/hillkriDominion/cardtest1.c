/*
 * cardtest1.c
 *
 
 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest1: cardtest1.c dominion.o rngs.o
 *      gcc -o cardtest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "initializeGame()"

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

    // ----------- TEST 1: Number of player = 1. --------------
    printf("TEST 1: Number of player = 1.\n");

    numPlayers = 1;
    returnValue = initializeGame(numPlayers, k, seed, &G);

    expectedReturnValue = -1;
    printf("Return value = %d, expected = %d\n", returnValue, expectedReturnValue);
    printAssert(returnValue == expectedReturnValue);


    // ----------- TEST 2: Kingdom cards are duplicated. --------------
    printf("TEST 2: Kingdom cards are duplicated.\n");

    k[0] = k[1]; 
    returnValue = initializeGame(numPlayers, k, seed, &G);

    expectedReturnValue = -1;
    numPlayers = 2;
    printf("Return value = %d, expected = %d\n", returnValue, expectedReturnValue);
    printAssert(returnValue == expectedReturnValue);
    k[0] = adventurer;

    // ----------- TEST 3: curse card. --------------
    printf("TEST 3: curse card.\n");

    for ( numPlayers = 2 ; numPlayers <= maxPlayers ; numPlayers++ ){
        printf("# Numbef of player = %d #\n", numPlayers);
        if (numPlayers == 2) expectedCount = 10;
        else if (numPlayers == 3) expectedCount = 20;
        else if (numPlayers == 4) expectedCount = 30;

        expectedReturnValue = initializeGame(numPlayers, k, seed, &G);

        printf("Curse card count = %d, expected = %d\n", G.supplyCount[curse], expectedCount);
        printAssert(G.supplyCount[curse] == expectedCount);    
    }

    // ----------- TEST 4: Victory card - estate. --------------
    printf("TEST 4: Victory card - estate.\n");

    for ( numPlayers = 2 ; numPlayers <= maxPlayers ; numPlayers++ ){
        printf("# Numbef of player = %d #\n", numPlayers);
        if (numPlayers == 2) expectedCount = 8;
        else if (numPlayers >= 3) expectedCount = 12;

        expectedReturnValue = initializeGame(numPlayers, k, seed, &G);

        printf("Victory card (estate) count = %d, expected = %d\n", G.supplyCount[estate], expectedCount);
        printAssert(G.supplyCount[estate] == expectedCount);    
    }

    // ----------- TEST 5: Treasure card - copper. --------------
    printf("TEST 5: Treasure card - copper.\n");
    for ( numPlayers = 2 ; numPlayers <= maxPlayers ; numPlayers++ ){
        printf("# Numbef of player = %d #\n", numPlayers);

        expectedReturnValue = initializeGame(numPlayers, k, seed, &G);

        printf("Treasure card (copper) count = %d, expected = %d\n", G.supplyCount[copper], 60 - numPlayers*7);
        printAssert(G.supplyCount[copper] == 60 - numPlayers*7);    
    }


    // ----------- TEST 6: Kingdom card (adventurer). --------------
    printf("TEST 6: Kingdom card (adventurer).\n");

    for ( numPlayers = 2 ; numPlayers <= maxPlayers ; numPlayers++ ){
        printf("# Numbef of player = %d #\n", numPlayers);

        expectedReturnValue = initializeGame(numPlayers, k, seed, &G);

        expectedCount = 10;
        printf("Kingdom card (adventurer) count = %d, expected = %d\n", G.supplyCount[adventurer], expectedCount);
        printAssert(G.supplyCount[adventurer] == expectedCount);    
    }


    // ----------- TEST 7: Other players' deck contains 3 copper and 7 estate. --------------
    printf("TEST 7: Other players' deck contains 3 copper and 7 estate.\n");

    int player = 0;
    numPlayers = 4;
    for ( player = 1 ; player < maxPlayers ; player++ ){
        printf("# Other player (player[%d]) #\n", player);
        initializeGame(numPlayers, k, seed, &G);

        printf("Copper count in deck = %d, expected = %d\n", getSpecificCountInDeck(&G, player, copper), 7);
        printAssert(getSpecificCountInDeck(&G, player, copper) == 7);
        printf("Estate count in deck = %d, expected = %d\n", getSpecificCountInDeck(&G, player, estate), 3);
        printAssert(getSpecificCountInDeck(&G, player, estate) == 3);
    }

    // ----------- TEST 8: Other players' hand count and discard count. --------------
    printf("TEST 8: Other players' hand count and discard count.\n");

    numPlayers = 4;
    for ( player = 1 ; player < maxPlayers ; player++ ){
        printf("# Other player (player[%d]) #\n", player);
        initializeGame(numPlayers, k, seed, &G);

        printf("Hand count = %d, expected = %d\n", G.handCount[player], 0);
        printAssert(G.handCount[player] == 0);
        printf("Dicard count = %d, expected = %d\n", G.discardCount[player], 0);
        printAssert(G.discardCount[player] == 0);
    }


    // ----------- TEST 9: Current players' hand count, discard count, deck count,number of buy, number of action. --------------
    printf("TEST 9: Current players' hand count, discard count, deck count,number of buy, number of action.\n");

    initializeGame(numPlayers, k, seed, &G);

    printf("Hand count = %d, expected = %d\n", G.handCount[thisPlayer], 5);
    printAssert(G.handCount[thisPlayer] == 5);

    printf("Dicard count = %d, expected = %d\n", G.discardCount[thisPlayer], 0);
    printAssert(G.discardCount[thisPlayer] == 0);

    printf("deck count = %d, expected = %d\n", G.deckCount[thisPlayer], 5);
    printAssert(G.deckCount[thisPlayer] == 5);

    printf("Number of buy = %d, expected = %d\n", G.numBuys, 1);
    printAssert(G.numBuys == 1);

    printf("Number of action = %d, expected = %d\n", G.numActions, 1);
    printAssert(G.numBuys == 1);


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