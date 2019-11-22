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
    if(currentPlayer != whoseTurn(pre) || handPos < 0 || handPos > pre->handCount[currentPlayer] - 1 ||
    pre->hand[currentPlayer][handPos] != baron){
        if(post.numBuys != pre->numBuys || post.handCount[currentPlayer] != pre->handCount[currentPlayer] ||
        post.numActions != pre->numActions || post.discardCount != pre->discardCount ||
        post.deckCount != pre->deckCount || supplyCount(estate, &post) != supplyCount(estate, pre) ||
        post.coins != pre->coins || post.playedCardCount != pre->playedCardCount)
            return 0;
        else
            return 1;
    }

    //make sure baron is put into played card pile and there is one more buy
    if(post.numBuys != pre->numBuys + 1 || post.numActions != pre->numActions - 1 ||
     post.deckCount != pre->deckCount || post.playedCardCount != pre->playedCardCount + 1)
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

        //player had an estate to discard and chose to do so
        else if( post.handCount[currentPlayer] != pre->handCount[currentPlayer] - 2 ||
        post.discardCount != pre->discardCount + 1 || post.coins != pre->coins + 4 || 
        supplyCount(estate, &post) != supplyCount(estate, pre) )
                return 0;
    }

    //player did not discard an estate
    if( !choice1 && (post.handCount[currentPlayer] != pre->handCount[currentPlayer] - 1 || post.coins != pre->coins ||
     (supplyCount(estate, pre) == 0 && (supplyCount(estate, &post) != 0 || post.discardCount != pre->discardCount) ) ||
     (supplyCount(estate, pre) > 0 && (supplyCount(estate, &post) != supplyCount(estate, pre) - 1 || post.discardCount != pre->discardCount + 1) ) ) )
        return 0;

    return 1;
}

int main(){
    srand(time(0));
    int passed = 0;
    int numTests = 0;
    int k[10] = { adventurer, council_room, feast, gardens, mine, 
    remodel, smithy, village, baron, great_hall };
    int t[3] = {copper, silver, gold};
    int v[4] = {curse, estate, duchy, province};

    time_t start = time(0);

    //randomly initialize the gameState and check if baronCardEffect works
    for(int i=0; i<numTests; i++){
        struct gameState G;

        //initialize the state randomly
        G.playedCardCount = rand();
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
        G.whoseTurn = rand() % G.numPlayers; //can be any player's turn
        G.coins = rand();
        G.numActions = rand();
        G.numBuys = rand();
        G.phase = 0;

        //decide the arguments
        int choice1 = rand() % 2; //boolean
        int handPos = 0;
        if(G.handCount[G.whoseTurn] != 0){
            int handPos = rand() % G.handCount[G.whoseTurn];
            if(G.hand[G.whoseTurn][handPos] != baron){
                for(int card=0; card<G.handCount[G.whoseTurn]; card++){
                    if(G.hand[G.whoseTurn][card] == baron){
                        handPos = card;
                        break;
                    }
                }
            }
        }
        passed += testBaronCardEffect(choice1, &G, handPos, G.whoseTurn);
    }

    //int timeTook = (time(0) - start);
    printf("%d of %d TESTS PASSED\n", passed, numTests);
    printf("IT TOOK %ld SECONDS TO RUN THESE TESTS\n", time(0) - start);

    return 0;

}