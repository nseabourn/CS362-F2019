CFLAGS= -Wall -fpic -coverage -lm -std=c99 -g

rngs.o: rngs.h rngs.c
	gcc -c rngs.c  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard  testDrawCard.c dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

unittest1: unittest1.c dominion.o rngs.o
	gcc -o unittest1 dominion.o rngs.o unittest1.c $(CFLAGS)
	echo "Result for running my unit tests:" > unittestresults.out
	echo "unittest1.c:" >> unittestresults.out
	./unittest1 >> unittestresults.out
	gcov dominion.c >> unittestresults.out -b -f
	cat dominion.c.gcov >> unittestresults.out

unittest2: unittest2.c dominion.o rngs.o
	gcc -o unittest2 dominion.o rngs.o unittest2.c $(CFLAGS)
	echo "Result for running my unit tests:" > unittestresults.out
	echo "unittest2.c:" >> unittestresults.out
	./unittest2 >> unittestresults.out
	gcov dominion.c >> unittestresults.out -b -f
	cat dominion.c.gcov >> unittestresults.out

unittestresults: unittest1.c unittest2.c unittest3.c unittest4.c unittest5.c dominion.o rngs.o
	gcc -o unittest1 dominion.o rngs.o unittest1.c $(CFLAGS)
	gcc -o unittest2 dominion.o rngs.o unittest2.c $(CFLAGS)
	gcc -o unittest3 dominion.o rngs.o unittest3.c $(CFLAGS)
	gcc -o unittest4 dominion.o rngs.o unittest4.c $(CFLAGS)
	gcc -o unittest5 dominion.o rngs.o unittest5.c $(CFLAGS)
	echo "Result for running my unit tests:" > tests.out
	echo "unittest1.c:" >> unittestresults.out
	./unittest1 >> unittestresults.out
	gcov bst.c >> unittestresults.out -b -f
	echo "\n\nunittest2.c:" >> unittestresults.out
	./unittest2 >> unittestresults.out
	gcov bst.c >> unittestresults.out -b -f
	echo "\n\nunittest3.c:" >> unittestresults.out
	./unittest3 >> unittestresults.out
	gcov bst.c >> unittestresults.out -b -f
	echo "\n\nunittest4.c:" >> unittestresults.out
	./unittest4 >> unittestresults.out
	gcov bst.c >> unittestresults.out -b -f
	echo "\n\nunittest5.c:" >> unittestresults.out
	./unittest5 >> unittestresults.out
	gcov bst.c >> unittestresults.out -b -f

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
