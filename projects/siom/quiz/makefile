CFLAGS= -Wall -fpic -coverage -lm -std=c99

execute: testme
	- ./testme
	gcov testme.c -b -f

testme: testme.c
	gcc -o testme -g  testme.c $(CFLAGS)

all: testme execute
	