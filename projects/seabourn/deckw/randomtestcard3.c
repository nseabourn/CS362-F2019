#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

int testTributeCardEffect(struct gameState* pre, int nextPlayer, int *tributeRevealedCards, int handPos, int currentPlayer){
    struct gameState post;
    memcpy (&post, pre, sizeof(struct gameState));
    tributeCardEffect(&post, nextPlayer, tributeRevealedCards, handPos, currentPlayer);

    //make sure tribute is put into played card pile andnothing changed that shouldn't have
    if(post.numBuys != pre->numBuys || post.playedCardCount != pre->playedCardCount + 1 ||
    post.discardCount[currentPlayer] != pre->discardCount[currentPlayer])
        return 0;
    for(int i=0; i<treasure_map; i++){
        if(post.supplyCount[i] != pre->supplyCount[i] || post.embargoTokens[i] != pre->embargoTokens[i])
            return 0;
    }

    if(pre->discardCount[nextPlayer] + pre->deckCount[nextPlayer] < 2 && tributeRevealedCards[0] != -1 && tributeRevealedCards[1] != -1)
        return 0;
    if(pre->discardCount[nextPlayer] + pre->deckCount[nextPlayer] >= 2 && (tributeRevealedCards[0] == -1 || tributeRevealedCards[1] == -1) )
        return 0;

    //no cards were revealed
    if(tributeRevealedCards[0] == -1){
        if(post.handCount[currentPlayer] != pre->handCount[currentPlayer] -1 ||
        post.numActions != pre->numActions - 1 || post.coins != pre->coins)
            return 0;
    }
    //only one card was revealed or two duplicate cards were revealed
    else if(tributeRevealedCards[1] == -1 || tributeRevealedCards[0] == tributeRevealedCards[1]){
        //victory card
        if(tributeRevealedCards[0] >= estate && tributeRevealedCards[0] <= province){
            if(post.coins != pre->coins || post.numActions != pre->numActions - 1 ||
            (pre->deckCount[currentPlayer] + pre->discardCount[currentPlayer] > 0 && post.handCount[currentPlayer] == pre->handCount[currentPlayer] - 1) )
                return 0;
        }
        //treasure card
        else if(tributeRevealedCards[0] >= copper && tributeRevealedCards[0] <= gold){
            if(post.coins != pre->coins + 2 || post.numActions != pre->numActions -1 ||
            post.handCount[currentPlayer] != pre->handCount[currentPlayer] - 1)
                return 0;
            
        }
        //action card
        else{
            if(post.coins != pre->coins || post.numActions != pre->numActions + 1 ||
            post.handCount[currentPlayer] != pre->handCount[currentPlayer] - 1)
                return 0;
        }

    }
    //two different cards were revealed
    else{
        int addCoin = 0;
        int addAct = 0;
        int addHand = 0;
        for(int i=0; i<2; i++){
            //victory card
            if(tributeRevealedCards[0] >= estate && tributeRevealedCards[0] <= province)
                addHand+=2;
            //treasure card
            else if(tributeRevealedCards[0] >= copper && tributeRevealedCards[0] <= gold)
                addCoin+=2;
            //action card
            else
                addAct+=2;
        }
        if(addHand < pre->discardCount[currentPlayer] + pre->deckCount[currentPlayer])
            addHand = pre->discardCount[currentPlayer] + pre->deckCount[currentPlayer];
        if(post.coins += pre->coins + addCoin || post.numActions != pre->numActions + addAct - 1 ||
        post.handCount[currentPlayer] != pre->handCount[currentPlayer] - 1 + addHand )
            return 0;
    }

    return 1;
}

int main(){
    printf("TESTING tributeCardEffect()\n");
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
        int nextPlayer = G.whoseTurn + 1;
        if(G.whoseTurn == G.numPlayers - 1)
            nextPlayer = 0;
        if(G.handCount[G.whoseTurn] == 0)
            G.handCount[G.whoseTurn] = 1;

        //decide the arguments
        int handPos = rand() % G.handCount[G.whoseTurn];
        G.hand[G.whoseTurn][handPos] = tribute;
        int tributeRevealedCards[2] = {-1, -1};

        passed += testTributeCardEffect(&G, nextPlayer, tributeRevealedCards, handPos, G.whoseTurn);
    }

    printf("%d of %d TESTS PASSED\n", passed, numTests);
    printf("IT TOOK %ld SECONDS TO RUN THESE TESTS\n\n", time(0) - start);

    return 0;

}