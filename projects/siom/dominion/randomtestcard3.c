#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

#define TESTCARD "tribute"

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
    int tributeRevealedCards[2] = {-1, -1};
    int cardType1, cardType2;

    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);

    // seeding
    srand(time(NULL));

    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
 
    for ( m = 1; m <= 27*27 ; m++){
        printf("---------------- Run %d ----------------\n",m);

        xtraCoins = 0;
        xtraAction = 0;
        draw = 0;
        cardType1 = 0;
        cardType2 = 0;

        // set gameState to random byte 
        for ( i = 0; i < sizeof(struct gameState);i++ ){
            ((char*)&G)[i] = floor(rand()*256);
        }

        // set number of player
        G.numPlayers = rand()%3 + 2;

        // set hand count, deck count, and discard count
        for ( i = 0 ; i < G.numPlayers ; i++){
            //if ( i == thisPlayer)
            G.handCount[i] = rand()%MAX_HAND+1;
            //else
            //    G.handCount[i] = rand()%2+4;   // for other players, limit the subset as boundary case

            G.deckCount[i] = rand()%3;
            G.discardCount[i] = rand()%3;
        }

        // randomly assign card to next player's deck 
        for ( i = 0 ; i < G.deckCount[thisPlayer + 1] ; i++){
            G.deck[thisPlayer + 1][i] = rand()%(treasure_map+1);
        }

        // randomly assign card to next player's discard
        for ( i = 0 ; i < G.discardCount[thisPlayer + 1] ; i++){
            G.discard[thisPlayer + 1][i] = rand()%(treasure_map+1);
        }

        // set current player's coins
        G.coins = rand()%(60+1)*1 + rand()%(40+1)*3 + rand()%(30+1)*6;

        // set current player's action
        G.numActions = rand()%10; 

        // make a copy for comprasion
        memcpy(&testG, &G, sizeof(struct gameState));

        // lets call the function
        tributeEffect(&testG, thisPlayer, thisPlayer + 1, tributeRevealedCards);

        // assertion
        printf("Next player: deck count = %d | hand count = %d \n", G.deckCount[thisPlayer + 1], G.discardCount[thisPlayer + 1]);
        for ( i = 0 ; i < G.deckCount[thisPlayer + 1] ; i++){
            printf("Next player: deck[%d] = %d \n", i,  G.deck[thisPlayer + 1][i]);
        }
        for ( i = 0 ; i < G.discardCount[thisPlayer + 1] ; i++){
            printf("Next player: discard[%d] = %d \n", i,  G.discard[thisPlayer + 1][i]);
        }

/*        if (G.deckCount[thisPlayer + 1] == 1 && G.discardCount[thisPlayer + 1] == 0){
            cardType1 = getCardType(G.deck[thisPlayer + 1][0]);
            cardType2 = 0;
        }
        else if (G.deckCount[thisPlayer + 1] == 0 && G.discardCount[thisPlayer + 1] == 1){
            cardType1 = getCardType(G.discard[thisPlayer + 1][0]);
            cardType2 = 0;
        }
        else if ((G.deckCount[thisPlayer + 1] + G.discardCount[thisPlayer + 1]) == 0){
            cardType1 = 0;
            cardType2 = 0;
        }
        else if ((G.deckCount[thisPlayer + 1] + G.discardCount[thisPlayer + 1]) >= 2){
            if (G.deckCount[thisPlayer + 1] ==)
        }*/

        for (i = 0; i <= 2; i ++){
            if (tributeRevealedCards[i] == copper || tributeRevealedCards[i] == silver || tributeRevealedCards[i] == gold){//Treasure cards
                xtraCoins += 2;
            }  
            else if (tributeRevealedCards[i] == estate || tributeRevealedCards[i] == duchy || tributeRevealedCards[i] == province || tributeRevealedCards[i] == gardens || tributeRevealedCards[i] == great_hall){//Victory Card Found
                draw += 2;
            }
            else if (tributeRevealedCards[i] >=0 ){//Action Card
                xtraAction += 2;
            }
        }

        printf("current player (player[0]): coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
        printAssert(testG.coins == G.coins + xtraCoins*2);

        printf("current player (player[0]): hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + draw);
        printAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + draw);


        printf("current player (player[0]): number of action = %d, expected = %d\n", testG.numActions, G.numActions + xtraAction);
        printAssert(testG.numActions == G.numActions + xtraAction);

        printf("\n");

    }

    printf("\n >>>>> Testing complete %s <<<<<\n\n", TESTCARD);


    return 0;
}

int getCardType(int card){
    if (card == copper || card == silver || card == gold){//Treasure cards
        return 1;
    }  
    else if (card == estate || card == duchy || card == province || card == gardens || card == great_hall){//Victory Card Found
        return  2;
    }
    else if (card >=0 ){//Action Card
        return 3;
    }

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