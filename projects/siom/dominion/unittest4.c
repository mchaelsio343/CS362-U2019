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

#define TESTCARD "minion"

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
    printf("current player (player[0]): coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins*2);
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

/*


    // ----------- TEST 2: choice2 = 1. Discard hand, redraw 4, other players with 5+ cards discard hand and draw 4. --------------
    printf("TEST 2: choice2 = 1. Discard hand, redraw 4, other players with 5+ cards discard hand and draw 4.\n");

    // set hand card
    setHandCards(&G, thisPlayer, minion, copper, copper, copper, copper);

    // set player[1] with only 3 cards on hand
    G.handCount[1] = 3;
    setHandCards(&G, 1, copper, copper, copper, -1, -1);
    G.deckCount[1] = 5;

    // set player[2] with only 4 cards on hand
    G.handCount[2] = 4;
    setHandCards(&G, 2, copper, copper, copper, copper, -1);
    G.deckCount[2] = 5;

    // set player[2] with only 5 cards on hand
    G.handCount[3] = 5;
    setHandCards(&G, 3, copper, copper, copper, copper, copper);
    G.deckCount[3] = 5;

    // copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    choice1 = 0;
    choice2 = 1;
    minionEffect(&testG, choice1, choice2, 0, thisPlayer);

    // current player, player[0] 
    xtraAction = 1;                           // from the effect
    discarded = testG.handCount[thisPlayer];  // discard hand
    draw = 4;
    printf("# current player# \n");
    printf("current player (player[0]): number of action = %d, expected = %d\n", testG.numActions, G.numActions + xtraAction);
    printAssert(testG.numActions == G.numActions + xtraAction);
    
    printf("current player (player[0]): hand count = %d, expected = %d\n", testG.handCount[thisPlayer], draw);
    printAssert(testG.handCount[thisPlayer] == draw);

    printf("current player (player[0]): deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - draw);
    printAssert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer] - draw);
    
    printf("current player (player[0]): coins = %d, expected = %d\n", testG.coins, G.coins);
    printAssert(testG.coins == G.coins);

    printf("current player (player[0]): dicard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + discarded);
    printAssert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + discarded);

    // other player, player[1]
    discarded = testG.handCount[1];
    printf("# other player who has 3 cards on hand # \n");
    printf("other player (player[1]): hand count = %d, expected = %d\n", testG.handCount[1], G.handCount[1]);
    printAssert(testG.handCount[1] == G.handCount[1]);

    printf("other player (player[1]): deck count = %d, expected = %d\n", testG.deckCount[1], G.deckCount[1]);
    printAssert(testG.deckCount[1] == G.deckCount[1]);

    printf("other player (player[1]): dicard count = %d, expected = %d\n", testG.discardCount[1], G.discardCount[1]);
    printAssert(testG.discardCount[1] == G.discardCount[1]);


    // other player, player[2]
    discarded = testG.handCount[2];
    printf("# other player who has 4 cards on hand # \n");
    printf("other player (player[2]): hand count = %d, expected = %d\n", testG.handCount[2], G.handCount[2]);
    printAssert(testG.handCount[2] == G.handCount[2]);

    printf("other player (player[2]): deck count = %d, expected = %d\n", testG.deckCount[2], G.deckCount[2]);
    printAssert(testG.deckCount[2] == G.deckCount[2]);

    printf("other player (player[2]): dicard count = %d, expected = %d\n", testG.discardCount[2], G.discardCount[2] + discarded);
    printAssert(testG.discardCount[2] == G.discardCount[2] + discarded);


    // other player, player[3]
    discarded = testG.handCount[3];
    printf("# other player who has 5 cards on hand # \n");
    printf("other player (player[3]): hand count = %d, expected = %d\n", testG.handCount[3], draw);
    printAssert(testG.handCount[3] == draw);

    printf("other player (player[3]): deck count = %d, expected = %d\n", testG.deckCount[3], G.deckCount[3] - draw);
    printAssert(testG.deckCount[3] == G.deckCount[3] - draw);

    printf("other player (player[3]): dicard count = %d, expected = %d\n", testG.discardCount[3], G.discardCount[3] + discarded);
    printAssert(testG.discardCount[3] == G.discardCount[3] + discarded);
*/

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