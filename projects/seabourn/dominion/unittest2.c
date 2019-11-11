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

void testMinion() {
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
    minionCardEffect(1, 0, &G, 0, currPlayer);
    // verify that your asserts pass, for the bugs your asserts may fail
    if(G.numActions != pre.numActions + 1
        || G.handCount[currPlayer] != pre.handCount[currPlayer] - 1
        || G.discardCount[currPlayer] != pre.discardCount[currPlayer] + 1
        || G.coins != pre.coins + 2)
        printf("Failed Test 1\n");

    //
    // set the state of your variables
    // Positive Test: discard hand, +4 cards, other players with >5 handsize discard hands and draw 4. Other player has handsize <5, deck is greater than 4
    //
    memcpy (&G, &cleanSlate, sizeof(struct gameState));
    discardHand(currPlayer, &G);
    G.handCount[!currPlayer] = 0;
    G.handCount[currPlayer] = 2;
    G.hand[currPlayer][0] = minion;
    G.hand[currPlayer][1] = gold;
    while(G.deckCount[currPlayer] < 5){
        G.deck[currPlayer][G.deckCount[currPlayer]] = gold;
        G.deckCount[currPlayer]++;
    }
    memcpy (&pre, &G, sizeof(struct gameState));
    // call the refactored function: int minionCardEffect(int choice1, int choice2, 
    //                         struct gameState *state, int handPos, int currentPlayer)
    minionCardEffect(0, 0, &G, 0, currPlayer);
    // verify that your asserts pass, for the bugs your asserts may fail
    if(G.numActions != pre.numActions
        || G.handCount[currPlayer] != 4
        || G.discardCount[currPlayer] != pre.discardCount[currPlayer] + pre.handCount[currPlayer]
        || G.coins != pre.coins
        || G.deckCount[currPlayer] != pre.deckCount[currPlayer] - 4
        || G.deckCount[!currPlayer] != pre.deckCount[!currPlayer]
        || G.handCount[!currPlayer] != pre.handCount[!currPlayer])
        printf("Failed Test 2\n");

    //
    // set the state of your variables
    // Positive Test: discard hand, +4 cards, other players with >5 handsize discard hands and draw 4. Other player has handsize >5, decks are greater than 4
    //
    memcpy (&G, &cleanSlate, sizeof(struct gameState));
    discardHand(currPlayer, &G);
    G.handCount[!currPlayer] = 6;
    for(int i=0; i<6; i++)
        G.hand[!currPlayer][i] = gold;
    G.handCount[currPlayer] = 2;
    G.hand[currPlayer][0] = minion;
    G.hand[currPlayer][1] = gold;
    while(G.deckCount[currPlayer] < 5){
        G.deck[currPlayer][G.deckCount[currPlayer]] = gold;
        G.deckCount[currPlayer]++;
    }
    while(G.deckCount[!currPlayer] < 5){
        G.deck[!currPlayer][G.deckCount[!currPlayer]] = gold;
        G.deckCount[!currPlayer]++;
    }
    memcpy (&pre, &G, sizeof(struct gameState));
    // call the refactored function: int minionCardEffect(int choice1, int choice2, 
    //                         struct gameState *state, int handPos, int currentPlayer)
    minionCardEffect(0, 0, &G, 0, currPlayer);
    // verify that your asserts pass, for the bugs your asserts may fail
    if(G.numActions != pre.numActions
        || G.handCount[currPlayer] != 4
        || G.handCount[!currPlayer] != 4
        || G.discardCount[currPlayer] != pre.discardCount[currPlayer] + pre.handCount[currPlayer]
        || G.discardCount[!currPlayer] != pre.discardCount[!currPlayer] + pre.handCount[!currPlayer]
        || G.coins != pre.coins
        || G.deckCount[currPlayer] != pre.deckCount[currPlayer] - 4
        || G.deckCount[!currPlayer] != pre.deckCount[!currPlayer] - 4)
        printf("Failed Test 3\n");

    //
    // set the state of your variables
    // Positive Test: discard hand, +4 cards, deck < 4, discard > 4
    //
    memcpy (&G, &cleanSlate, sizeof(struct gameState));
    discardHand(currPlayer, &G);
    G.handCount[!currPlayer] = 0;
    G.handCount[currPlayer] = 2;
    G.hand[currPlayer][0] = minion;
    G.hand[currPlayer][1] = gold;
    G.deckCount[currPlayer] = 1;
    G.discardCount[currPlayer] = 5;
    for(int i=0; i<4; i++)
        G.discard[currPlayer][i] = gold;
    memcpy (&pre, &G, sizeof(struct gameState));
    // call the refactored function: int minionCardEffect(int choice1, int choice2, 
    //                         struct gameState *state, int handPos, int currentPlayer)
    minionCardEffect(0, 0, &G, 0, currPlayer);
    // verify that your asserts pass, for the bugs your asserts may fail
    if(G.numActions != pre.numActions
        || G.handCount[currPlayer] != 4
        || G.discardCount[currPlayer] != 1
        || G.coins != pre.coins
        || G.deckCount[currPlayer] != pre.deckCount[currPlayer] + pre.discardCount[currPlayer] + pre.handCount[currPlayer] - 1 - 4
        || G.handCount[!currPlayer] != pre.handCount[!currPlayer])
        printf("Failed Test 4\n");

    //
    // set the state of your variables
    // Positive Test: discard hand, +4 cards, discard + deck < 4
    //
    memcpy (&G, &cleanSlate, sizeof(struct gameState));
    discardHand(currPlayer, &G);
    G.handCount[!currPlayer] = 0;
    G.handCount[currPlayer] = 1;
    G.hand[currPlayer][0] = minion;
    G.deckCount[currPlayer] = 1;
    G.discardCount[currPlayer] = 0;
    memcpy (&pre, &G, sizeof(struct gameState));
    // call the refactored function: int minionCardEffect(int choice1, int choice2, 
    //                         struct gameState *state, int handPos, int currentPlayer)
    minionCardEffect(0, 0, &G, 0, currPlayer);
    // verify that your asserts pass, for the bugs your asserts may fail
    if(G.numActions != pre.numActions
        || G.handCount[currPlayer] != pre.discardCount[currPlayer] + pre.deckCount[currPlayer]
        || G.handCount[!currPlayer] != 4
        || G.deckCount[currPlayer] != 0
        || G.discardCount[currPlayer] != 0
        || G.coins != pre.coins)
        printf("Failed Test 5\n");


    printf("Test completed!\n");
}

int main () {
    testMinion();
    exit(0);
}
