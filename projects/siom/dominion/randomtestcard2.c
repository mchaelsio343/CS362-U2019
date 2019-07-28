#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

#define TESTCARD "minion"

int main() {
    int newCards = 0;
    int discarded = 1;
    int xtraCoins = 0;
    int xtraAction = 0;
    int draw = 0;
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
    int exception[] = {-1};
    int numberOfEstate;

    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);

    // seeding
    srand(time(NULL));

    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
 
    for ( m = 1; m <= 100 ; m++){
        printf("---------------- Run %d ----------------\n",m);

        // gain 2 coins
        choice1 = rand()%2;

        // discard hand and draw 4
        choice2 = rand()%2;
        
        // set gameState to random byte 
        for ( i = 0; i < sizeof(struct gameState);i++ ){
            ((char*)&G)[i] = floor(rand()*256);
        }

        // set number of player
        G.numPlayers = rand()%3 + 2;

        // set hand count, deck count, and discard count
        for ( i = 0 ; i < G.numPlayers ; i++){
            if ( i == thisPlayer)
                G.handCount[i] = rand()%MAX_HAND+1;
            else
                G.handCount[i] = rand()%2+4;   // for other players, limit the subset as boundary case

            G.deckCount[i] = rand()%(MAX_DECK+1);
            G.discardCount[i] = rand()%(MAX_DECK+1);
        }

        // set current player's coins
        G.coins = rand()%(60+1)*1 + rand()%(40+1)*3 + rand()%(30+1)*6;

        // set current player's action
        G.numActions = rand()%10; 

        // set hand card randomly with exception
        assignRandomCardToHand(&G, thisPlayer, exception);

        // set the playing minion card to a random hand position
        handpos = assignPlayCardToRandomPos(&G, thisPlayer, minion);

        // make a copy for comprasion
        memcpy(&testG, &G, sizeof(struct gameState));

        // lets call the function
        //minionEffect(&testG, choice1, thisPlayer);
        minionEffect(&testG, choice1, choice2, handpos, thisPlayer);

        // assertion
        printf("choice1 = %d | choice2 = %d \n", choice1, choice2);
        printf("Total players = %d\n", G.numPlayers);
        for ( i = 0; i < G.numPlayers ; i++ ){
            if ( i == thisPlayer)
                printf("Current player: player[%d] handCount = %d \n", i, G.handCount[i]);
            else
                printf("Other player: player[%d] handCount = %d \n", i, G.handCount[i]);
        }
        printf("\n");

        if ( choice1 == 1){
            xtraCoins = 2;
            xtraAction = 1;
            printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
            printAssert(testG.coins == G.coins + xtraCoins);

            printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] - discarded);
            printAssert(testG.handCount[thisPlayer] == (G.handCount[thisPlayer] - discarded));

            printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);
            printAssert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer]);

            printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + discarded);
            printAssert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + discarded);

            printf("number of action = %d, expected = %d\n", testG.numActions, G.numActions + xtraAction);
            printAssert(testG.numActions == G.numActions + xtraAction);
        }
        else if (choice2 == 1){
            for( i = 0; i < G.numPlayers ; i++ ){
                // current player
                if ( i == thisPlayer){
                    xtraAction = 1;                 // from the effect
                    discarded = testG.handCount[i]; // discard hand
                    draw = 4;                       // from the effect
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
                }
                // other player who has handCount >= 5
                else if ( G.handCount[i] >= 5 ){
                    draw = 4;       // from the effect
                    discarded = testG.handCount[i];
                    printf("# other player who has 5 cards on hand # \n");
                    printf("other player (player[%d]): hand count = %d, expected = %d\n", i, testG.handCount[3], draw);
                    printAssert(testG.handCount[i] == draw);

                    printf("other player (player[%d]): deck count = %d, expected = %d\n", i, testG.deckCount[3], G.deckCount[3] - draw);
                    printAssert(testG.deckCount[i] == G.deckCount[i] - draw);

                    printf("other player (player[%d]): dicard count = %d, expected = %d\n", i, testG.discardCount[3], G.discardCount[3] + discarded);
                    printAssert(testG.discardCount[i] == G.discardCount[i] + discarded);
                }
                // other player who has handCount <= 4
                else if ( G.handCount[i] <= 4){
                    discarded = 0;
                    printf("# other player who has 4 cards on hand # \n");
                    printf("other player (player[%d]): hand count = %d, expected = %d\n", i, testG.handCount[i], G.handCount[i]);
                    printAssert(testG.handCount[i] == G.handCount[i]);

                    printf("other player (player[%d]): deck count = %d, expected = %d\n", i, testG.deckCount[i], G.deckCount[i]);
                    printAssert(testG.deckCount[i] == G.deckCount[i]);

                    printf("other player (player[%d]): dicard count = %d, expected = %d\n", i, testG.discardCount[i], G.discardCount[i] + discarded);
                    printAssert(testG.discardCount[i] == G.discardCount[i] + discarded);
                }
            }
        }
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

int assignRandomCardToHand(struct gameState *G, int player, int exception[], int playingCard){
    int i, j;
    int randomCard;
    int exceptionFlag;
    //if (exception[0] != -1)
    size_t n = sizeof(exception)/sizeof(exception[0]);

    //printf("here!!! n = %d \n\n", n);

    for ( i = 0; i < G->handCount[player] ; i++ ){  
       //printf("inside for loop!!!\n\n");
       do{
            //printf("inside while loop!!!\n\n");
            exceptionFlag = 0;
            randomCard = rand() % (treasure_map+1);
            //printf("randomCard = %d", randomCard);
            for( j = 0 ; j < n ; j++){
                if (randomCard == exception[j]){
                    exceptionFlag = 1;
                    break;
                }
            }
       }while(exceptionFlag);

       // random is not the one in exceptinon; assign it to the hand
       G->hand[player][i] = randomCard;

    }

    //printf("end of assignRandomCardToHand!!!\n\n");
}

int assignPlayCardToRandomPos(struct gameState *G, int player, int playingCard){
    int handPos = rand()%G->handCount[player];
    G->hand[player][handPos] = playingCard;
    return handPos;
}

void assignSpecificCardToHand(struct gameState *G, int player, int numberOfCard, int card){
    int count;
    int collision;
    int index;

    if (G->handCount[player] <= 1)
        return;

    if (numberOfCard > G->handCount[player])
        numberOfCard = G->handCount[player]-1;

    for( count = 0 ; count < numberOfCard ; count++ ){
        do{
            collision = 0;
            index = rand()%G->handCount[player];
            //printf("index = %d \n", index);
            //printf("handcount = %d\n",G->handCount[player]);
            //printf("card[0]=%d\n", G->hand[player][0]);
            //printf("card[1]=%d\n", G->hand[player][1]);            
            //printHand(G,player);
            if ( G->hand[player][index] == card ) {
                collision = 1;
                //printf("collision\n");
                //printf(" G->hand[player][index] = %d, card = %d \n\n", G->hand[player][index], card);
            }

        }while(collision);

        G->hand[player][index] = card;
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

int printHand(struct gameState *G, int player){
    int i;
    for (i = 0; i < G->handCount[player] ; i++){
        printf("G.hand[%d][%d] = %d \n\n", player, i, G->hand[player][i]);
    }
}