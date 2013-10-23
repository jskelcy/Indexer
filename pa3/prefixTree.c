#include "prefixTree.h"

int hash(char c) {
		if ('0' <= c && c <= '9') {
				return (c - '0');
		} else if ('a' <= c && c <= 'z') {
				return (c - 'a' + 10);
		} else if ('A' <= c && c <= 'Z') {
				return (c - 'A' + 10);
		} else {
				return -1;
		}
}

treeRoot* treeInit() {
		treeRoot *returnTree = (treeRoot *) calloc (1, sizeof(treeRoot));
		returnTree->root = (Node *) calloc(1, sizeof(Node));
		returnTree->ptr = returnTree->root;
		return returnTree;
}

void insertNode(treeRoot *tree,char c) {
		int index = hash(c);
		if (index == -1) {
			return;
		}
		if (tree->ptr->branches == NULL) {
				tree->ptr->branches = (Node **) calloc(36, sizeof(Node*));
		}
		if (tree->ptr->branches[index] == NULL) {
				tree->ptr->branches[index] = (Node *) calloc(1, sizeof(Node));
				tree->ptr->branches[index]->letter = tolower(c);
				tree->ptr = tree->ptr->branches[index];
		} else {
				tree->ptr = tree->ptr->branches[index];
		}
}

void freeBranches(Node *curr) {
		int i;
		if (curr->branches != NULL) {
				for (i = 0; i < 36; i++) {
						if (curr->branches[i] != NULL) {
								freeBranches(curr->branches[i]);
						}
				}
				free(curr->branches);
		}
		if (curr->freak != NULL) {
				FLDestroy(curr->freak);
		}
		free(curr);
}

void freeList(FilenameNode *curr) {
	if (curr != NULL) {
		freeList(curr->next);
		if (curr->filename != NULL) {
			free(curr->filename);
		}
		free(curr);
	}
}

void freeTree(treeRoot *tree) {
	freeBranches(tree->root);
	freeList(tree->filenames);
	free(tree);
}

char *append(char *s, char c) {
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
		for(i=0; i<length; i++){
				copy[i]=s[i];
		}
		copy[length]= c;
		copy[length + 1] ='\0';
		return copy;
}

void printTree(treeRoot *tree, char *currString, FILE *openFile, int depth) {
	Node *curr = tree->ptr;
	int i;
	if (curr->isWord == 1 && depth != 0) {
		fprintf(openFile,"<list> ");
		fprintf(openFile, "%s\n", currString);
		FLPrint(curr->freak, openFile);
		fprintf(openFile,"</list>\n");
	}
	if (curr->branches != NULL) {
		currString[depth + 1] = '\0';
		for (i = 0; i < 36; i++) {
			if (curr->branches[i] != NULL) {
				/*char *newWord = append(currString, curr->branches[i]->letter);*/
				currString[depth] = curr->branches[i]->letter;
				tree->ptr = curr->branches[i];
				printTree(tree, currString, openFile, depth + 1);
				tree->ptr = curr;
			}
		}
		currString[depth] = '\0';
	}
}
