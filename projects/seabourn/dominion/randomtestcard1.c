#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

int testBaronCardEffect(int choice1, struct gameState *pre, int handPos, int currentPlayer){
    struct gameState post;
    memcpy (&post, pre, sizeof(struct gameState));
    baronCardEffect(choice1, &post, handPos, currentPlayer);

    //make sure card we're playing is in fact a baron and currentPlayer is the actual current player
    if(currentPlayer != whoseTurn(pre) || handPos < 0 || handPos > pre->handCount - 1 ||
    pre->hand[currentPlayer][handPos] != baron){
        if(post.numBuys != pre->numBuys || post.handCount[currentPlayer] != pre->handCount[currentPlayer] ||
        post.numActions != pre->numActions || post.discardCount != pre->discardCount ||
        post.deckCount != pre->deckCount || supplyCount(estate, &post) != supplyCount(estate, pre) ||
        post.coins != pre->coins)
            return 0;
        return 1;
    }

    //make sure there is one more buy
    if(post.numBuys != pre->numBuys + 1)
        return 0;

    //player chose to discard estate
    if(choice1){
        int hadEstate = 0;
        for(int i=0; i<pre->handCount[currentPlayer]; i++){
            if(pre->hand[currentPlayer][i] == estate){
                hadEstate = 1;
                break;
            }
        }
        //if player did not have an estate, should act as if player chose not to discard an estate
        if(!hadEstate)
            choice1 = 0;
        else{
            
        }
    }

    //player chose not to discard estate
    if(!choice1){

    }
}

int main(){
    time_t t = time(0);
    srand(time(0));
    int passed = 0;
    int numTests = 100;
    int k[10] = { adventurer, council_room, feast, gardens, mine, 
    remodel, smithy, village, baron, great_hall };
    int t[3] = {copper, silver, gold};
    int v[4] = {curse, estate, duchy, province};

    struct gameState G;
    int choice1, handPos;

    //randomly initialize the gameState and check if baronCardEffect works
    for(int i=0; i<numTests; i++){
        //initialize the state randomly
        for(int card=0; card<G.playedCardCount; card++){
            int rando = rand() % 17;
            if(rando < 10)
                G.playedCards[card] = k[rando];
            else if(rando < 13)
                G.playedCards[card] = t[rando - 10];
            else
                G.playedCards[card] = v[rando - 13];
        }
        G.numPlayers = rand() % (MAX_PLAYERS - 1) + 2;
        G.whoseTurn = rand() % G.numPlayers; //can be any player's turn
        for(int player=0; player<G.numPlayers; player++){
            //random hand
            G.handCount[player] = rand() % (MAX_HAND + 1);
            for(int card=0; card<G.handCount[player]; card++){
                int rando = rand() % 17;
                if(rando < 10)
                    G.hand[player][card] = k[rando];
                else if(rando < 13)
                    G.hand[player][card] = t[rando - 10];
                else
                    G.hand[player][card] = v[rando - 13];
            }
            //random discard pile
            G.discardCount[player] = rand() % (MAX_DECK + 1);
            for(int card=0; card<G.discardCount[player]; card++){
                int rando = rand() % 17;
                if(rando < 10)
                    G.discard[player][card] = k[rando];
                else if(rando < 13)
                    G.discard[player][card] = t[rando - 10];
                else
                    G.discard[player][card] = v[rando - 13];
            }
            //random deck
            G.deckCount[player] = rand() % (MAX_DECK + 1);
            for(int card=0; card<G.deckCount[player]; card++){
                int rando = rand() % 17;
                if(rando < 10)
                    G.deck[player][card] = k[rando];
                else if(rando < 13)
                    G.deck[player][card] = t[rando - 10];
                else
                    G.deck[player][card] = v[rando - 13];
            }
        }
        for(int pile = 0; pile<treasure_map; pile++){
            G.supplyCount[pile] = rand();
        }
        G.coins = rand();
        G.numActions = rand();
        G.numBuys = rand();
        G.playedCardCount = rand();
        G.phase = 0;

        //decide the arguments
        choice1 = rand() % 2; //boolean
        handPos = rand() % G.handCount[G.whoseTurn];
        passed += testBaronCardEffect(choice1, &G, handPos, G.whoseTurn);
    }

    printf("%d of %d TESTS PASSED", passed, numTests);
    printf("IT TOOK %f SECONDS TO RUN THESE TESTS", (time(0) - t)*1000);
    
    return 0;
}