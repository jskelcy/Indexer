#include <stdlib.h>
#include <stdio.h>
#include "prefixTree.h"

int hash(char c){
    int hashInt = (int) c;
    if(97<= hashInt && hashInt <= 122){
        return (hashInt - 97);
    }else{
        return (hashInt-48+10);
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
        tree->ptr->branches = (Node**) calloc (36,sizeof(Node*));
        tree->ptr->branches[index] = (Node*) calloc(1,sizeof(Node));
        tree->ptr->branches[index]->letter = c;
        tree->ptr = tree->ptr->branches[index];
        fprintf(stderr,"just insert [%c]\n",c);
    }else  if(tree->ptr->branches[index]== NULL){
            tree->ptr->branches[index] = (Node *) calloc (1,sizeof(Node));
            tree->ptr->branches[index]->letter = c;
            tree->ptr= tree->ptr->branches[index];
            fprintf(stderr,"just insert [%c]\n",c);
        }else{
            tree->ptr = tree->ptr->branches[index];
            fprintf(stderr,"just insert [%c]\n",c);
        }
}

