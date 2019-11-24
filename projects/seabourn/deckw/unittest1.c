#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 1

void testBaron() {
    // set your card array
    int k[10] = { adventurer, council_room, feast, gardens, mine, 
    remodel, smithy, village, baron, great_hall };
    // declare/intialize the game states
    struct gameState G;
    struct gameState pre;
    struct gameState cleanSlate;
    memset(&cleanSlate, 23, sizeof(struct gameState)); // set the cleanSlate
    srand(time(0));
    int seed = rand();
    initializeGame(2, k, seed, &cleanSlate);
    int currPlayer = cleanSlate.whoseTurn;
    printf("Begin Testing baronCardEffect():\n");

    //
    // set the state of your variables
    // Positive Test: discard estate in hand
    //
    memcpy (&G, &cleanSlate, sizeof(struct gameState));
    discardHand(currPlayer, &G);
    G.handCount[currPlayer] = 2;
    G.hand[currPlayer][0] = estate;
    G.hand[currPlayer][1] = baron;
    memcpy (&pre, &G, sizeof(struct gameState));
    // call the refactored function: int baronCardEffect(int choice1, struct gameState *state, 
    //                                                          int handPos, int currentPlayer);
    baronCardEffect(1, &G, 1, currPlayer);
    // verify that your asserts pass, for the bugs your asserts may fail
    if(G.numBuys != pre.numBuys + 1
        || G.handCount[currPlayer] != pre.handCount[currPlayer] - 2
        || G.discardCount[currPlayer] != pre.discardCount[currPlayer] + 2
        || G.coins != pre.coins + 4)
        printf("Failed Test 1\n");

    //
    // set the state of your variables
    // Positive Test: gain estate to hand
    //
    memcpy (&G, &cleanSlate, sizeof(struct gameState));
    discardHand(currPlayer, &G);
    G.handCount[currPlayer] = 2;
    G.hand[currPlayer][0] = estate;
    G.hand[currPlayer][1] = baron;
    G.supplyCount[estate] = 10;
    memcpy (&pre, &G, sizeof(struct gameState));
    // call the refactored function: int baronCardEffect(int choice1, struct gameState *state, 
    //                                                          int handPos, int currentPlayer);
    baronCardEffect(0, &G, 1, currPlayer);
    // verify that your asserts pass, for the bugs your asserts may fail
    if(G.numBuys != pre.numBuys + 1
        || G.supplyCount[estate] != pre.supplyCount[estate] - 1
        || G.handCount[currPlayer] != pre.handCount[currPlayer]
        || G.hand[currPlayer][0] != estate
        || G.discardCount[currPlayer] != pre.discardCount[currPlayer] + 1
        || G.coins != pre.coins)
        printf("Failed Test 2\n");

    //
    // set the state of your variables
    // Positive Test: don't gain or discard estate
    //
    memcpy (&G, &cleanSlate, sizeof(struct gameState));
    discardHand(currPlayer, &G);
    G.handCount[currPlayer] = 2;
    G.hand[currPlayer][0] = estate;
    G.hand[currPlayer][1] = baron;
    G.supplyCount[estate] = 0;
    memcpy (&pre, &G, sizeof(struct gameState));
    // call the refactored function: int baronCardEffect(int choice1, struct gameState *state, 
    //                                                          int handPos, int currentPlayer);
    baronCardEffect(0, &G, 1, currPlayer);
    // verify that your asserts pass, for the bugs your asserts may fail
    if(G.numBuys != pre.numBuys + 1
        || G.supplyCount[estate] != pre.supplyCount[estate]
        || G.handCount[currPlayer] != pre.handCount[currPlayer] - 1
        || G.discardCount[currPlayer] != pre.discardCount[currPlayer] + 1
        || G.coins != pre.coins)
        printf("Failed Test 3\n");

    // set the state of your variables
    // Negative Test: try to discard estate when don't have one in hand, but do have a card in hand and supply is empty
    //
    memcpy (&G, &cleanSlate, sizeof(struct gameState));
    discardHand(currPlayer, &G);
    G.handCount[currPlayer] = 2;
    G.hand[currPlayer][0] = gold;
    G.hand[currPlayer][1] = baron;
    G.supplyCount[estate] = 0;
    memcpy (&pre, &G, sizeof(struct gameState));
    // call the refactored function: int baronCardEffect(int choice1, struct gameState *state, 
    //                                                          int handPos, int currentPlayer);
    baronCardEffect(1, &G, 1, currPlayer);
    // verify that your asserts pass, for the bugs your asserts may fail
    if(G.numBuys != pre.numBuys + 1
        || G.supplyCount[estate] != pre.supplyCount[estate]
        || G.handCount[currPlayer] != pre.handCount[currPlayer] - 1
        || G.discardCount[currPlayer] != pre.discardCount[currPlayer] + 1
        || G.coins != pre.coins)
        printf("Failed Test 4\n");

    // set the state of your variables
    // Negative Test: try to discard estate when don't have one in hand, but do have a card in hand and supply is not empty
    //
    memcpy (&G, &cleanSlate, sizeof(struct gameState));
    discardHand(currPlayer, &G);
    G.handCount[currPlayer] = 2;
    G.hand[currPlayer][0] = gold;
    G.hand[currPlayer][1] = baron;
    G.supplyCount[estate] = 10;
    memcpy (&pre, &G, sizeof(struct gameState));
    // call the refactored function: int baronCardEffect(int choice1, struct gameState *state, 
    //                                                          int handPos, int currentPlayer);
    baronCardEffect(1, &G, 1, currPlayer);
    // verify that your asserts pass, for the bugs your asserts may fail
    if(G.numBuys != pre.numBuys + 1
        || G.supplyCount[estate] != pre.supplyCount[estate] - 1
        || G.handCount[currPlayer] != pre.handCount[currPlayer]
        || G.discardCount[currPlayer] != pre.discardCount[currPlayer] + 1
        || G.coins != pre.coins)
        printf("Failed Test 5\n");
    int hasEstate = 0;
    for(int i=0; i<G.handCount[currPlayer]; i++){
        if(G.hand[currPlayer][i] == estate)
            hasEstate = 1;
    }
    if(!hasEstate)
        printf("Failed Test 5\n");

    // set the state of your variables
    // Negative Test: try to discard estate when don't have a hand  and supply is empty
    //
    memcpy (&G, &cleanSlate, sizeof(struct gameState));
    discardHand(currPlayer, &G);
    G.handCount[currPlayer] = 1;
    G.hand[currPlayer][0] = baron;
    G.supplyCount[estate] = 0;
    memcpy (&pre, &G, sizeof(struct gameState));
    // call the refactored function: int baronCardEffect(int choice1, struct gameState *state, 
    //                                                          int handPos, int currentPlayer);
    baronCardEffect(1, &G, 1, currPlayer);
    // verify that your asserts pass, for the bugs your asserts may fail
    if(G.numBuys != pre.numBuys + 1
        || G.supplyCount[estate] != pre.supplyCount[estate]
        || G.handCount[currPlayer] != pre.handCount[currPlayer] - 1
        || G.discardCount[currPlayer] != pre.discardCount[currPlayer] + 1
        || G.coins != pre.coins)
        printf("Failed Test 6\n");

    // set the state of your variables
    // Negative Test: try to discard estate when don't have a hand and supply is not empty
    //
    memcpy (&G, &cleanSlate, sizeof(struct gameState));
    discardHand(currPlayer, &G);
    G.handCount[currPlayer] = 1;
    G.hand[currPlayer][0] = baron;
    G.supplyCount[estate] = 10;
    memcpy (&pre, &G, sizeof(struct gameState));
    // call the refactored function: int baronCardEffect(int choice1, struct gameState *state, 
    //                                                          int handPos, int currentPlayer);
    baronCardEffect(1, &G, 1, currPlayer);
    // verify that your asserts pass, for the bugs your asserts may fail
    if(G.numBuys != pre.numBuys + 1
        || G.supplyCount[estate] != pre.supplyCount[estate] - 1
        || G.handCount[currPlayer] != pre.handCount[currPlayer]
        || G.hand[currPlayer][0] != estate
        || G.discardCount[currPlayer] != pre.discardCount[currPlayer] + 1
        || G.coins != pre.coins)
        printf("Failed Test 7\n");


    printf("Test completed!\n");
}

int main () {
    testBaron();
    exit(0);
}
