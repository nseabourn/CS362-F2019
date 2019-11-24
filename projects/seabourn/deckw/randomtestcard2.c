#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

int testMinionCardEffect(int choice1, int choice2, struct gameState *pre, int handPos, int currentPlayer){
    struct gameState post;
    memcpy (&post, pre, sizeof(struct gameState));
    minionCardEffect(choice1, choice2, &post, handPos, currentPlayer);

    //make sure minion is put into played card pile and there is one more action and nothing changed that shouldn't have
    if(post.numBuys != pre->numBuys || post.numActions != pre->numActions ||
     post.playedCardCount != pre->playedCardCount + 1)
        return 0;
    for(int i=0; i<treasure_map; i++){
        if(post.supplyCount[i] != pre->supplyCount[i] || post.embargoTokens[i] != pre->embargoTokens[i])
            return 0;
    }

    //make sure no cards from within each player's deck, hand, and discard pile go elsewhere
    if( post.deckCount[currentPlayer] + post.discardCount[currentPlayer] + post.handCount[currentPlayer] !=
    pre->deckCount[currentPlayer] + pre->discardCount[currentPlayer] + pre->handCount[currentPlayer] -1 )
        return 0;
    for(int i=0; i<post.numPlayers; i++){
        if( post.deckCount[i] + post.discardCount[i] + post.handCount[i] !=
        pre->deckCount[i] + pre->discardCount[i] + pre->handCount[i] )
            return 0;

    }

    //player chose to get 2 coins
    if(choice1){
        if(post.coins != pre->coins + 2 || post.handCount[currentPlayer] != pre->handCount[currentPlayer] - 1)
            return 0;
        for(int player = 0; player<post.numPlayers; player++){
            if(player != currentPlayer && post.handCount[player] != pre->handCount[player])
                return 0;
            if(post.deckCount[player] != pre->deckCount[player] || post.discardCount[player] != pre->discardCount[player])
                return 0;
        }
    }
    //player chose to discard hand
    else{
        if(post.coins != pre->coins)
            return 0;

        //didn't have to shuffle discard pile into deck
        if(pre->deckCount[currentPlayer] >= 4){
            if(post.handCount[currentPlayer] != 4 || post.deckCount[currentPlayer] != pre->deckCount[currentPlayer] - 4 ||
            post.discardCount[currentPlayer] != pre->discardCount[currentPlayer] + pre->handCount[currentPlayer] - 1)
                return 0;
        }
        //had to shuffle
        else{
            if(post.discardCount[currentPlayer] != 0)
                return 0;
            int numCards = pre->handCount[currentPlayer] + pre->deckCount[currentPlayer] + pre->discardCount[currentPlayer];
            if(numCards > 4){
                if(post.handCount[currentPlayer] != 4)
                    return 0;
            }
            else if(post.handCount[currentPlayer] != numCards - 1)
                    return 0;
        }
        for(int player=0; player<post.numPlayers; player++){
            if(player != currentPlayer){
                if(pre->handCount[player] >= 5){
                    if(post.handCount[player] != 4)
                        return 0;
                    //didn't have to shuffle discard pile into deck
                    if(pre->deckCount[player] >= 4){
                        if(post.deckCount[player] != pre->deckCount[player] - 4 ||
                        post.discardCount[player] != pre->discardCount[player] + pre->handCount[player])
                            return 0;
                    }
                    //had to shuffle
                    else if(post.discardCount[player] != 0)
                        return 0;
                }
                else if(post.handCount[player] != pre->handCount[player] ||
                post.deckCount[player] != pre->deckCount[player] || post.discardCount[player] != pre->discardCount[player])
                    return 0;
            }
        }
    }

    return 1;
}

int main(){
    printf("TESTING minionCardEffect()\n");
    srand(time(0));
    int passed = 0;
    int numTests = 1000000;
    int k[10] = { adventurer, council_room, feast, gardens, mine, 
    remodel, smithy, village, minion, great_hall };
    int t[3] = {copper, silver, gold};
    int v[4] = {curse, estate, duchy, province};

    time_t start = time(0);

    //randomly initialize the gameState and check if minionCardEffect works
    for(int i=0; i<numTests; i++){
        struct gameState G;

        //initialize the state randomly
        G.playedCardCount = rand() % MAX_DECK;
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
            G.handCount[player] = rand() % MAX_HAND;
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
            G.discardCount[player] = rand() % MAX_DECK;
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
            G.deckCount[player] = rand() % MAX_DECK;
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
            G.supplyCount[pile] = rand() % MAX_DECK;
        }
        G.coins = rand();
        G.numActions = rand();
        G.numBuys = rand();
        G.phase = 0;
        G.whoseTurn = rand() % G.numPlayers; //can be any player's turn
        if(G.handCount[G.whoseTurn] == 0)
            G.handCount[G.whoseTurn] = 1;

        //decide the arguments
        int choice1 = rand() % 2; //boolean
        int choice2 = rand() % 2; //also a boolean
        int handPos = rand() % G.handCount[G.whoseTurn];
        G.hand[G.whoseTurn][handPos] = minion;

        passed += testMinionCardEffect(choice1, choice2, &G, handPos, G.whoseTurn);
    }

    printf("%d of %d TESTS PASSED\n", passed, numTests);
    printf("IT TOOK %ld SECONDS TO RUN THESE TESTS\n\n", time(0) - start);

    return 0;

}