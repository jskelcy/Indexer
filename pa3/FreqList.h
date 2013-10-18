#ifndef __FreqList_h__
#define __FreqList_h__

#include <stdlib.h>
#include <stdio.h>

typedef struct FileNode FileNode;
typedef struct FreqNode FreqNode;
typedef struct FreqList FreqList;

struct FileNode {
	char *name;
	int length;
	FileNode *prev, *next;
	FreqNode *parent;
};

struct FreqNode {
	int freq;
	FreqNode *prev, *next;
	FileNode *child;
};

struct FreqList {
	FreqNode *root;
	FileNode *current;
};

FileNode *CreateFileNode();
FreqNode *CreateFreqNode();
FreqList *FLCreate();
void FLInsert(FreqList *, char *);
void FLPrintf(FreqList *, FILE *);
void FLPrint(FreqList *);
void FLDestroy(FreqList *);

#endif
