/*
 * unittest3.c
 *
 
 */

/*
 * Include the following lines in your makefile:
 *
 * unittest3: unittest3.c dominion.o rngs.o
 *      gcc -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "ambassador"

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


    // ----------- TEST 1 - 5 : choice2 = [-1, 3]. Test different number of returned card. --------------
    printf("TEST 1 - 5 : choice2 = [-1, 3]. Test different number of returned card.\n");
    
    // set hand card so that there is enough card to return
    setHandCards(&G, thisPlayer, ambassador, copper, copper, copper, copper);

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // set choice1 and choice3 with a different value
    choice1 = 1; 
    choice3 = 0;

    for ( i = -1 ; i <= 3 ; i++){
        choice2 = i;
        printf("TEST %d: choice2 = %d. Test different number of returned card.\n", i + 2, choice2);
        returnValue = ambassadorEffect(&testG, choice1, choice2, choice3, thisPlayer);

        if ( i >= 0 && i <= 2 ) expectedReturnValue = 0;
        else expectedReturnValue = -1;

        printf("Return value = %d, expected = %d\n", returnValue, expectedReturnValue);
        printAssert(returnValue == expectedReturnValue);
    }


    // ----------- TEST 6 : choice1 = choice3. Revealed card position is the current player card position. --------------
    printf("TEST 6 : choice1 = choice3. Revealed card position is the current player card position.\n");
    
    choice1 = choice3; 
    expectedReturnValue = -1;
    returnValue = ambassadorEffect(&testG, choice1, choice2, choice3, thisPlayer);

    printf("Return value = %d, expected = %d\n", returnValue, expectedReturnValue);
    printAssert(returnValue == expectedReturnValue);


    // ----------- TEST 7 : choice2 = 2. Not enought card to return. --------------
    printf("TEST 7 : choice2 = 2. Not enought card to return.\n");

    // set hand card so that there is not enough card to return
    setHandCards(&G, thisPlayer, ambassador, copper, silver, silver, silver);

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    choice1 = 1;
    choice2 = 2;
    choice3 = 0;
    expectedReturnValue = -1;
    returnValue = ambassadorEffect(&testG, choice1, choice2, choice3, thisPlayer);

    printf("Return value = %d, expected = %d\n", returnValue, expectedReturnValue);
    printAssert(returnValue == expectedReturnValue);


    // ----------- TEST 8 : choice2 = 2. Enought card to return. --------------
    printf("TEST 8 : choice2 = 2. Enought card to return.\n");

    // set hand card so that there is not enough card to return
    setHandCards(&G, thisPlayer, ambassador, copper, copper, copper, silver);

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    choice1 = 1;
    choice2 = 2;
    choice3 = 0;
    discarded = 1;
    expectedReturnValue = -1;
    returnValue = ambassadorEffect(&testG, choice1, choice2, choice3, thisPlayer);

    printf("Card Supply Count = %d, expected = %d\n", testG.supplyCount[copper], G.supplyCount[copper] + choice2 - (numPlayers - 1));
    printAssert(testG.supplyCount[copper] == G.supplyCount[copper] + choice2);

    printf("# current player # \n");
    printf("current player (player[0]): hand count =  %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] - choice2 - discarded);
    printAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] - choice2 - discarded);

    for( i = 1 ; i <= 3 ; i++){
        printf("# other player # \n");
        printf("other player (player[%d]): dicard count = %d, expected = %d\n", i, testG.discardCount[i], G.discardCount[i] + 1);
        printAssert(testG.discardCount[i] == G.discardCount[i] + 1);
    }

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