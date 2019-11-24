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
    printf("Begin Testing minionCardEffect():\n");

    //
    // set the state of your variables
    // Positive Test: +2
    //
    memcpy (&G, &cleanSlate, sizeof(struct gameState));
    discardHand(currPlayer, &G);
    G.handCount[currPlayer] = 2;
    G.hand[currPlayer][0] = minion;
    G.hand[currPlayer][1] = gold;
    memcpy (&pre, &G, sizeof(struct gameState));
    // call the refactored function: int minionCardEffect(int choice1, int choice2, 
    //                         struct gameState *state, int handPos, int currentPlayer)
    minionCardEffect(1, 0, G, 0, currPlayer);
    // verify that your asserts pass, for the bugs your asserts may fail
    if(G.numActions != pre.numActions + 1
        || G.handCount[currPlayer] != pre.handCount[currPlayer] - 1
        || G.discardCount[currPlayer] != pre.discardCount[currPlayer] + 1
        || G.coins != pre.coins + 2)
        printf("Failed Test 1\n");


    printf("Test completed!\n");
}

int main () {
    testBaron();
    exit(0);
}
