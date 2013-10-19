CCFlags= -ansi -Wall -pedantic -g -std=c99 -D_BSD_SOURCE

CC= gcc

all: index

index: prefixTree.o index.o FreqList.o prefixTree.h FreqList.h
	${CC} ${CCFlags} -o index prefixTree.o index.o FreqList.o prefixTree.h FreqList.h

PrefixTree.o: prefixTree.c
	${CC} ${CCFlags} -c prefixTree.c

FreqList.o: FreqList.c
	${CC} ${CCFlags} -c FreqList.c

index.o: index.c
	${CC} ${CCFlags} -c index.c

clean:
	rm *o index
