CCFlags= -ansi -Wall -pedantic -g -std=c99 -D_BSD_SOURCE

CC= gcc

all: dirtrav

dirtrav: prefixTree.o dirtrav.o FreqList.o prefixTree.h FreqList.h
	${CC} ${CCFlags} -o dirtrav prefixTree.o dirtrav.o FreqList.o prefixTree.h FreqList.h

PrefixTree.o: prefixTree.c
	${CC} ${CCFlags} -c prefixTree.c

FreqList.o: FreqList.c
	${CC} ${CCFlags} -c FreqList.c

dirtrav.o: dirtrav.c
	${CC} ${CCFlags} -c dirtrav.c
