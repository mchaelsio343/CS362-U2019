#ifndef _DOMINION_HELPERS_H
#define _DOMINION_HELPERS_H

#include "dominion.h"

int drawCard(int player, struct gameState *state);
int updateCoins(int player, struct gameState *state, int bonus);
int discardCard(int handPos, int currentPlayer, struct gameState *state, 
		int trashFlag);
int gainCard(int supplyPos, struct gameState *state, int toFlag, int player);
int getCost(int cardNumber);
int cardEffect(int card, int choice1, int choice2, int choice3, 
	       struct gameState *state, int handPos, int *bonus);

// Refactored card effect functions
int baronEffect(struct gameState *state, int discardEstate, int currentPlayer);
int minionEffect(struct gameState *state, int gain2Coins, int discardToDraw4, int handPos, int currentPlayer);
int ambassadorEffect(struct gameState *state, int revealedCardPos, int numToReturn, int handPos, int currentPlayer);
int tributeEffect(struct gameState *state, int currentPlayer, int nextPlayer, int* tributeRevealedCards);
int mineEffect(struct gameState *state, int discardCardPos, int treasure, int handPos, int currentPlayer);

// new helper functions
void discardHand(struct gameState *state, int player);
void drawCards(struct gameState *state, int player, int numbersOfCard);
void dicardDeckTop(struct gameState *state, int player);
void moveDiscardToDeck(struct gameState *state, int player);
#endif
