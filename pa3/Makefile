CCFlags= -ansi -Wall -pedantic -g -std=c99 -D_BSD_SOURCE

CC= gcc

all: indexer

indexer: prefixTree.o indexer.o FreqList.o prefixTree.h FreqList.h
	${CC} ${CCFlags} -o indexer prefixTree.o indexer.o FreqList.o prefixTree.h FreqList.h

PrefixTree.o: prefixTree.c
	${CC} ${CCFlags} -c prefixTree.c

FreqList.o: FreqList.c
	${CC} ${CCFlags} -c FreqList.c

indexer.o: indexer.c
	${CC} ${CCFlags} -c indexer.c

clean:
	rm *o indexer dummy.txt
