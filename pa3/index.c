#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "prefixTree.h"
#include "FreqList.h"
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * These are helper functions, which help determine alphaNumeric-ness
 * and concatentate directories together.
 */
int isAlphaNum(char c){
		return (('a'<= c && c <='z') || ('0' <= c && c <='9') || ('A' <= c && c <= 'Z'));
}

char *concatDir(char *c1, char *c2) {
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
		c3 = calloc(len1 + len2 + addSlash + 1, sizeof(char));
		for (; i < len1; i++) {
				c3[i] = c1[i];
		}
		if (addSlash) {
				c3[i] = '/';
				i++;
		}
		for (i = 0; i < len2; i++) {
				c3[len1 + addSlash + i] = c2[i];
		}
		c3[len1 + addSlash + i] = '\0';
		return c3;
}

/*
 * This is for travesing a single file.
 * As it traverses the file, it also traverses the PrefixTree and FrequencyList.
 */
void travfile(treeRoot *tree, char *filename) {
	FILE *file = fopen(filename, "r");
	char currChar;
	int depth = 0;
	while ((currChar = fgetc(file)) != EOF) {
		if (isAlphaNum(currChar)) {
			insertNode(tree, currChar);
			depth++;
		} else {
			tree->ptr->isWord = 1;
			if (tree->ptr->freak == NULL) {
				tree->ptr->freak = FLCreate();
			}
			FLInsert(tree->ptr->freak, filename);
			tree->ptr = tree->root;
			if (tree->depth < depth) {
				tree->depth = depth;
			}
			depth = 0;
		}
	}
	if (tree->ptr != tree->root) {
		tree->ptr->isWord = 1;
		if (tree->ptr->freak == NULL) {
			tree->ptr->freak = FLCreate();
		}
		FLInsert(tree->ptr->freak, filename);
		tree->ptr = tree->root;
		if (tree->depth < depth) {
			tree->depth = depth;
		}
		depth = 0;
	}
	fclose(file);
}

/*
 * This is for traversing a single directory.
 * Should the directory actually be a filename, it will travfile immediately instead.
 */
void travdir(treeRoot *tree, char *dirRoot) {
		DIR *dp = opendir(dirRoot);
		struct dirent *dirItem;
		char *currPath;
		if (dp == NULL) {
			travfile(tree, dirRoot);
		} else {
			while ((dirItem = readdir(dp)) != NULL) {
				if (dirItem->d_name[0] == '.') {
						continue;
				}
				if (dirItem->d_type & DT_DIR) {
						currPath = concatDir(dirRoot, dirItem->d_name);
						travdir(tree, currPath);
						free(currPath);
				} else {
						FilenameNode *FNN = calloc(1, sizeof(FilenameNode));
						currPath = concatDir(dirRoot, dirItem->d_name);
						FNN->filename = currPath;
						FNN->next = tree->filenames;
						tree->filenames = FNN;
						travfile(tree, currPath);
				}
			}
			closedir(dp);
		}
		tree->ptr = tree->root;
		return;
}

int main(int argc, char **argv){
		char ans, *printBuffer;
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
			ans = 'y';
			printf("%c\n", ans);
			/*scanf("%c",&ans);
			if (ans != 'y'){
				fclose(fp);
				return 0;
			}*/
		}
		if (fp != NULL) {
			fclose(fp);
		}
		/* Do program */
		treeRoot *tree = treeInit();
		travdir(tree, argv[2]);
		fp = fopen(argv[1], "w");
		printBuffer = malloc((tree->depth + 1) * sizeof(char));
		printTree(tree, printBuffer, fp, 0);
		free(printBuffer);
		freeTree(tree);
		fclose(fp);
		return 0;
}
