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
		Node *curr = tree->ptr;
		if(curr->isWord == 1 && currString != NULL){
				fprintf(openFile,"<list> ");
				fprintf(openFile, "%s\n\n", currString);
				FLPrintf(curr->freak, openFile);
				fprintf(openFile,"\n</list>\n\n");
		}
		if (curr->branches != NULL) {
				for (int i = 0; i < 62; i++) {
						if (curr->branches[i] != NULL) {
								char *newWord = append(currString, curr->branches[i]->letter);
								tree->ptr = curr->branches[i];
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
		if(curr->branches != NULL){
				for (int i = 0; i<62; i++){
						if(curr->branches[i]!= NULL){
								tree->ptr = curr->branches[i];
								freeTree(tree);
								tree->ptr = curr;
						}
				}
				free(curr->branches);
		}
		if (curr->freak != NULL) {
				FLDestroy(curr->freak);
		}
		free(curr);
}


int isAlphaNum(char c){
		return (('a'<= c && c <='z') || ('0' <= c && c <='9') || ('A' <= c && c <= 'Z'));
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
		if(dp == NULL){
				FILE *file = fopen(dirRoot,"r");
				currChar = fgetc(file);
				while(currChar != EOF){
						if (isAlphaNum(currChar)) {
								insertNode(tree, currChar);
						} else {
								tree->ptr->isWord = 1;
								if (tree->ptr->freak == NULL) {
										tree->ptr->freak = FLCreate();
								}
								FLInsert(tree->ptr->freak,dirRoot);
								tree->ptr = tree->root;
						}
						currChar = fgetc(file);
				}
				fclose(file);
				return;
		}
		while((currFile = readdir(dp)) != NULL){
				if(currFile->d_name[0]=='.'){
						continue;
				}
				if(currFile->d_type & DT_DIR){
						fileName= concatDIR(currPath,currFile->d_name);
						travdir(tree,fileName);
						free(fileName);
				}else{
						FILE *file = fopen((fileName=concatDIR(currPath,currFile->d_name)),"r");
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
						free(fileName);
						fclose(file);
				}
		}
		closedir(dp);
		tree->ptr=tree->root;
		return;
}

int  main(int argc, char** argv){
		char ans;
		FILE *fp = NULL;
		DIR *dp = NULL;
		/* Check for valid argc */
		if (argc != 3) {
				fprintf(stderr, "Invalid syntax. Please use the following format:\n\t./indexer filename file_or_folder_ToSearch\n");
				return 1;
		}
		/* Check for valid second argv */
		fp = fopen(argv[2],"r");
		dp = opendir(argv[2]);
		if (fp == NULL && dp == NULL) {
			fprintf(stderr, "The second argument is invalid - %s does not exist.\n", argv[2]);
			return 1;
		}
		if (fp != NULL) {
			fclose(fp);
		}
		if (dp != NULL) {
			closedir(dp);
		}
		/* Check for valid first argv */
		dp = opendir(argv[1]);
		if (dp != NULL) {
			fprintf(stderr, "The first argument is invalid - %s is a directory. Please use a filename.\n", argv[1]);
			closedir(dp);
			return 1;
		}
		/* Check for redundant names */
		if (strcmp(argv[1], argv[2]) == 0) {
			fprintf(stderr, "Both arguments are the same. Cannot overwrite.\n");
			return 1;
		}
		/* Check for overwrite permission */
		fp = fopen(argv[1],"r");
		if (fp != NULL) {
			printf("Are you sure you want to overwrite %s? (y/n)\n",argv[1]);
			scanf("%c",&ans);
			if(ans != 'y'){
				fclose(fp);
				printf("Exiting.\n");
				return 0;
			}
		}
		if (fp != NULL) {
			fclose(fp);
		}
		/* Do program */
		fp = fopen(argv[1],"w");
		treeRoot *tree = treeInit();
		travdir(tree, argv[2]);
		printTree(tree, NULL,fp);
		freeTree(tree);
		free(tree);
		fclose(fp);
		printf("Done. Exiting.\n");
		return 0;
}
