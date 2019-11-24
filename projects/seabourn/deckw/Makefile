CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

randomtestcard1: randomtestcard1.c dominion.o rngs.o
	gcc -o randomtestcard1 -g randomtestcard1.c dominion.o rngs.o $(CFLAGS)

randomtestcard2: randomtestcard2.c dominion.o rngs.o
	gcc -o randomtestcard2 -g randomtestcard2.c dominion.o rngs.o $(CFLAGS)

randomtestcard3: randomtestcard3.c dominion.o rngs.o
	gcc -o randomtestcard3 -g randomtestcard3.c dominion.o rngs.o $(CFLAGS)

randomtestresults: randomtestcard1 randomtestcard2 randomtestcard3
	echo "Result for running random tests:\n-------------------------------" > randomtestresults.out
	echo "-------------------------------\nrandomtestcard1:\n-------------------------------" >> randomtestresults.out
	./randomtestcard1 >> randomtestresults.out
	echo "-------------------------------\nrandomtestcard2:\n-------------------------------" >> randomtestresults.out
	./randomtestcard2 >> randomtestresults.out
	echo "-------------------------------\nrandomtestcard3:\n-------------------------------" >> randomtestresults.out
	./randomtestcard3 >> randomtestresults.out
	gcov -b -f dominion.c >> randomtestresults.out

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
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe unittest1 unittest2 unittest3 randomtestcard1 randomtestcard2 randomtestcard3 randomtestresults
