CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
#testDrawCard: testDrawCard.c dominion.o rngs.o
#	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)


#########

#badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
#	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

#testBuyCard: testDrawCard.c dominion.o rngs.o
#	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

#testAll: dominion.o testSuite.c
#	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

#interface.o: interface.h interface.c
#	gcc -c interface.c -g  $(CFLAGS)

#runtests: testDrawCard 
#	./testDrawCard &> unittestresult.out
#	gcov dominion.c >> unittestresult.out
#	cat dominion.c.gcov >> unittestresult.out

#unittestresults:	
#	gcc -o unittest1 dominion.c rngs.c unittest1.c $(CFLAGS)
#	unittest1 >> unittestresults.out
#	gcov dominion.c -b -f >> unittestresults.out
#	gcc -o unittest2 dominion.c rngs.c unittest2.c $(CFLAGS)
#	unittest2 >> unittestresults.out
#	gcov dominion.c -b -f >> unittestresults.out
#	gcc -o unittest3 dominion.c rngs.c unittest3.c $(CFLAGS)
#	unittest3 >> unittestresults.out
#	gcov dominion.c -b -f >> unittestresults.out
#	gcc -o unittest4 dominion.c rngs.c unittest4.c $(CFLAGS)
#	unittest4 >> unittestresults.out
#	gcov dominion.c -b -f >> unittestresults.out
#	gcc -o unittest5 dominion.c rngs.c unittest5.c $(CFLAGS)
#	unittest5 >> unittestresults.out
#	gcov dominion.c -b -f >> unittestresults.out
#	gcc -o cardtest1 dominion.c rngs.c cardtest1.c $(CFLAGS)
#	cardtest1 >> unittestresults.out
#	gcov dominion.c -b -f >> unittestresults.out
#	gcc -o cardtest2 dominion.c rngs.c cardtest2.c $(CFLAGS)
#	cardtest2 >> unittestresults.out
#	gcov dominion.c -b -f >> unittestresults.out
#	gcc -o cardtest3 dominion.c rngs.c cardtest3.c $(CFLAGS)
#	cardtest3 >> unittestresults.out
#	gcov dominion.c -b -f >> unittestresults.out
#	gcc -o cardtest4 dominion.c rngs.c cardtest4.c $(CFLAGS)
#	cardtest4 >> unittestresults.out
#	gcov dominion.c -b -f >> unittestresults.out
#	gcc -o cardtest5 dominion.c rngs.c cardtest5.c $(CFLAGS)
#	cardtest5 >> unittestresults.out
#	gcov dominion.c -b -f >> unittestresults.out
#
randomtestresults: 
	gcc -o randomtestcard1 dominion.c rngs.c randomtestcard1.c $(CFLAGS)
	randomtestcard1 >> randomtestresults.out
	gcov dominion.c -b -f >> randomtestresults.out
	gcc -o randomtestcard2 dominion.c rngs.c randomtestcard2.c $(CFLAGS)
	randomtestcard2 >> randomtestresults.out
	gcov dominion.c -b -f >> randomtestresults.out
	gcc -o randomtestcard3 dominion.c rngs.c randomtestcard3.c $(CFLAGS)
	randomtestcard3 >> randomtestresults.out
	gcov dominion.c -b -f >> randomtestresults.out
#player: player.c interface.o
#	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

#all: playdom player 

all: dominion.o playdom randomtestresults

clean:
	rm -f *.o randomcardtest1 randomcardtest2 randomcardtest3 unittest1 unittest2 unittest3 unittest4 unittest5 cardtest1 cardtest2 cardtest3 cardtest4 cardtest5 playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
