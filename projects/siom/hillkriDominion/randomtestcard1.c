#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

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
    int exception[] = {estate};
    int numberOfEstate;

    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);

    // seeding
    srand(time(NULL));

    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
 
    for ( m = 1; m <= 50 ; m++){
        printf("---------------- Run %d ----------------\n",m);


        choice1 = rand()%2;
        

        // set gameState to random byte 
        for ( i = 0; i < sizeof(struct gameState);i++ ){
            ((char*)&G)[i] = floor(rand()*256);
        }

        // set number of player
        G.numPlayers = rand()%3 + 2;

        // set the supply of estate
        G.supplyCount[estate] = rand()%(12+1);

        // set hand count
        G.handCount[thisPlayer] = rand()%MAX_HAND+1;

        // set deck count
        G.deckCount[thisPlayer] = rand()%(MAX_DECK+1);

        // set discard count
        G.discardCount[thisPlayer] = rand()%(MAX_DECK+1);

        // set current player's coins
        G.coins = rand()%(60+1)*1 + rand()%(40+1)*3 + rand()%(30+1)*6;

        numberOfEstate = rand()%G.handCount[thisPlayer];

        // set hand card randomly with exception
        assignRandomCardToHand(&G, thisPlayer, exception);

        // put randomly number of estate to the hand
        assignSpecificCardToHand(&G, thisPlayer, numberOfEstate, estate);

        // make a copy for comprasion
        memcpy(&testG, &G, sizeof(struct gameState));

        // let call the function
        baronEffect(&testG, choice1, thisPlayer);

        // assertion
        printf("choice1 = %d | estate on hand = %d | estate in supply = %d \n\n", choice1, numberOfEstate,  G.supplyCount[estate]);
        if ( choice1 == 1 && getSpecificHandCount(&testG, thisPlayer, estate) > 0){
            printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] - discarded);
            printAssert(testG.handCount[thisPlayer] == (G.handCount[thisPlayer] - discarded));
            
            printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);
            printAssert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer]);
            
            xtraCoins = 4;
            printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
            printAssert(testG.coins == G.coins + xtraCoins);

            printf("dicard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + discarded);
            printAssert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + discarded);
        }
        else{
            newCards = 1;
            discarded = 1; // played card
            printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] - discarded);
            printAssert(testG.handCount[thisPlayer] == (G.handCount[thisPlayer] - discarded));
            
            printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer]);
            printAssert(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer]);
            
            printf("coins = %d, expected = %d\n", testG.coins, G.coins);
            printAssert(testG.coins == G.coins);

            printf("dicard count = %d, expected = %d\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + discarded );
            printAssert(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + discarded);
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

void assignRandomCardToHand(struct gameState *G, int player, int exception[]){
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

/*void printHand(struct gameState *G, int player){
    int i;
    for(i = 0 ; i < 5; i++){
        printf("Card %d on hand = %d\n", i+1, G->hand[player][i]);
    }
}*/

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