#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include "prefixTree.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "FreqList.h"

char* append(char *s,char c){
    char *copy;
    int i;
    if(s == NULL){
        copy = (char *) malloc(sizeof(char)*2);
        copy[0]= c;
        copy[1]= '\0';
        return copy;
    }
    int length = strlen(s);
    copy = (char *)malloc(length+2);
    for(i=0;i<length;i++){
        copy[i]=s[i];
    }
    copy[length]= c;
    copy[length +1] ='\0';
    return copy;
}


void printTree(treeRoot* tree,char *currString,FILE *openFile){
    if(tree->ptr->isWord == 1){
        fprintf(openFile,"<list> ");
        fprintf(openFile, "%s\n\n", currString);
        FLPrintf(tree->ptr->freak, openFile);
        fprintf(openFile,"\n</list>\n\n");
    }
    if (tree->ptr->branches != NULL) {
        for (int i = 0; i < 36; i++) {
            if (tree->ptr->branches[i] != NULL) {
                Node *curr = tree->ptr;
                char *newWord = append(currString, tree->ptr->branches[i]->letter);
                tree->ptr = tree->ptr->branches[i];
                printTree(tree, newWord, openFile);
                free(newWord);
                tree->ptr = curr;
            }
        }
    }
}

void freeTree(treeRoot *tree){
    Node *curr;
    curr = tree->ptr;
    if(tree->ptr->branches != NULL){
        for (int i = 0; i<36; i++){
            if(tree->ptr->branches[i]!= NULL){
                tree->ptr = tree->ptr->branches[i];
                freeTree(tree);
                tree->ptr = curr;
            }
        }
        free(tree->ptr->branches);
    }
    free(curr);
}


int isAlphaNum(char c){
    int test = (int) tolower(c);
    if ((97<= test && test <=122)||(48 <= test && test<=57)){
        return 1;
    }else{
        return 0;
    }
}

char *concatDIR(char *c1, char *c2) {
    int len1 = 0, len2 = 0, i = 0, addSlash;
    char *c3;
    if (c1 == NULL || c1[0] == '\0' || c2 == NULL || c2[0] == '\0') {
        return NULL;
    }
    while (c1[len1] != '\0') {
        addSlash = c1[len1] != '/';
        len1++;
    }
    while (c2[len2] != '\0') {
        len2++;
    }
    c3 = calloc(len1 + len2 + 1 + addSlash, sizeof(char));
    for (; i < len1; i++) {
        c3[i] = c1[i];
    }
    if (addSlash) {
        c3[i] = '/';
        i++;
    }
    for (i = 0; i < len2; i++) {
        c3[len1 + i + addSlash] = c2[i];
    }
    c3[i + len1 + addSlash] = '\0';
    return c3;
}

void travdir(treeRoot *tree, char *dirRoot){
    DIR *dp;
    char *currPath  = dirRoot;
    char *fileName;
    char currChar;
    struct dirent *currFile;
    dp = opendir(dirRoot);
    printf("COMMENT: trying to add stuff to our tree\n");
    while((currFile = readdir(dp)) != NULL){
        if(currFile->d_name[0]=='.'){
            printf("COMMENT: skipping dot\n");
            continue;
        }
        if(currFile->d_type & DT_DIR){
            printf("COMMENT: found a directory %s to recurse\n", currPath);
            fileName= concatDIR(currPath,currFile->d_name);
            fprintf(stderr,"pushable[%s]\n",fileName);
            travdir(tree,fileName);
            free(fileName);
        }else{
            printf("COMMENT: Creating new filename\n");
            FILE *file = fopen((fileName=concatDIR(currPath,currFile->d_name)),"r");
            fprintf(stderr,"the file is: [%s]\n",fileName);
            currChar = fgetc(file);
            while(currChar != EOF){
                if(isAlphaNum(currChar)){
                    insertNode(tree, currChar);
                }else{
                    tree->ptr->isWord = 1;
                    if (tree->ptr->freak == NULL) {
                        tree->ptr->freak = FLCreate();
                    }
                    FLInsert(tree->ptr->freak,fileName);
                    tree->ptr = tree->root;
                }
            currChar = fgetc(file);
            }
        fclose(file);
        }
    }
    closedir(dp);
    tree->ptr=tree->root;
    printf("COMMENT: done recurse\n");
    return;
}

int  main(int argc, char** argv){
    char ans;
    FILE *fp = fopen(argv[2],"w");
    if(fp!= NULL){
        printf("the file %s is already a file, do you want to overwrite (y/n)\n",argv[2]);
        scanf("%c",&ans);
        if(ans == 'n'){
            fclose(fp);
            printf("KTHXBYE\n");
            return 0;
        }
    }
    treeRoot *tree = treeInit();
    travdir(tree, argv[1]);
    char *currString = malloc(sizeof(char));
    currString[0] = '\0';
    printTree(tree, currString,fp);
    freeTree(tree);
    return 0;
}

