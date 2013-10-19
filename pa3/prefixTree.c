#include <stdlib.h>
#include <stdio.h>
#include "prefixTree.h"

int hash(char c){
    if ('0' <= c && c <= '9') {
        return (c - '0');
    } else if ('a' <= c && c <= 'z') {
        return (c - 'a' + 10);
    } else if ('A' <= c && c <= 'Z') {
		return (c - 'A' + 36);
	} else {
		return 0;
	}
}

treeRoot* treeInit(){
    treeRoot *returnTree = (treeRoot *) calloc (1,sizeof(treeRoot));
    returnTree->root = (Node *) calloc(1,sizeof(Node));
    returnTree->ptr = returnTree->root;
    return returnTree;
}


void insertNode(treeRoot *tree,char c){
    c = tolower(c);
    int index = hash(c);
    if(tree->ptr->branches == NULL){
        tree->ptr->branches = (Node**) calloc (62,sizeof(Node*));
        tree->ptr->branches[index] = (Node*) calloc(1,sizeof(Node));
        tree->ptr->branches[index]->letter = c;
        tree->ptr = tree->ptr->branches[index];
    }else  if(tree->ptr->branches[index]== NULL){
            tree->ptr->branches[index] = (Node *) calloc (1,sizeof(Node));
            tree->ptr->branches[index]->letter = c;
            tree->ptr= tree->ptr->branches[index];
        }else{
            tree->ptr = tree->ptr->branches[index];
        }
}

