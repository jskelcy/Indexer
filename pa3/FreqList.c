#include "FreqList.h"

FileNode *CreateFileNode() {
	FileNode *file_node = malloc(sizeof(FileNode));
	file_node->name = NULL;
	file_node->prev = NULL;
	file_node->next = NULL;
	file_node->parent = NULL;
	return file_node;
}

FreqNode *CreateFreqNode() {
	FreqNode *freq_node = malloc(sizeof(FreqNode));
	freq_node->freq = 0;
	freq_node->prev = NULL;
	freq_node->next = NULL;
	freq_node->child = NULL;
	return freq_node;
}

FreqList *FLCreate() {
	FreqList *freq_list = malloc(sizeof(FreqList));
	freq_list->root = CreateFreqNode();
	freq_list->root->freq = 1;
	freq_list->current = NULL;
	return freq_list;
}

void FLInsert(FreqList *freq_list, char *filename) {
	int createNewNode = 0, length = 0;
	FileNode *file_node;
	/* if filename is nothing */
	if (filename == NULL || filename[0] == '\0') {
		return;
	}
	/* if the freq_list does not have a current node */
	while (filename[length] != '\0') {
		length++;
	}
	if (freq_list->current == NULL) {
		createNewNode = 1;
	} else {
		/* if length does not equal each other */
		if (length != freq_list->current->length) {
			createNewNode = 1;
		/* if the strings do not equal each other */
		} else {
			int index;
			char *base_string = freq_list->current->name;
			for (index = 0; index < length; index++) {
				if (base_string[index] != filename[index]) {
					createNewNode = 1;
					break;
				}
			}
		}
	}
	if (createNewNode) {
		/* Push new node */
		file_node = CreateFileNode();
		file_node->name = filename;
		file_node->length = length;
		file_node->parent = freq_list->root;
		file_node->next = freq_list->root->child;
		if (file_node->next != NULL) {
			file_node->next->prev = file_node;
		}
		freq_list->root->child = file_node;
		freq_list->current = file_node;
	} else {
		/* Free from previous list */
		file_node = freq_list->current;
		if (file_node->next != NULL) {
			file_node->next->prev = file_node->prev;
		}
		if (file_node->prev != NULL) {
			file_node->prev->next = file_node->next;
		} else {
			file_node->parent->child = file_node->next;
		}
		file_node->prev = NULL;
		file_node->next = NULL;
		/* Place in new list */
		if (file_node->parent->next == NULL) {
			file_node->parent->next = CreateFreqNode();
			file_node->parent->next->freq = file_node->parent->freq + 1;
			file_node->parent->next->prev = file_node->parent;
		}
		file_node->parent = file_node->parent->next;
		file_node->next = file_node->parent->child;
		if (file_node->next != NULL) {
			file_node->next->prev = file_node;
		}
		file_node->parent->child = file_node;
	}
}

void FLPrintf(FreqList *freq_list, FILE *fp) {
	FreqNode *curr = NULL, *front;
	for (front = freq_list->root; front != NULL; front = front->next) {
		curr = front;
	}
	for (; curr != NULL; curr = curr->prev) {
		FileNode *ptr;
		for (ptr = curr->child; ptr != NULL; ptr = ptr->next) {
			if (fp) {
				fprintf(fp, "%s %d ", ptr->name, curr->freq);
			} else {
				printf("\tFilename: %s\n\t\tCount: %d\n", ptr->name, curr->freq);
			}
		}
	}
	if (fp) {
		fprintf(fp, "\n");
	}
}

void FLPrint(FreqList *freq_list) {
	FLPrintf(freq_list, NULL);
}

void FLDestroy(FreqList *freq_list) {
	FreqNode *curr = NULL, *front;
	for (front = freq_list->root; front != NULL; front = front->next) {
		curr = front;
	}
	for (; curr != NULL; curr = front) {
		FileNode *ptr, *prev = NULL;
		for (ptr = curr->child; ptr != NULL; ptr = ptr->next) {
			prev = ptr;
		}
		ptr = prev;
		for (; ptr != NULL; ptr = prev) {
			prev = ptr->prev;
			free(ptr);
		}
		front = curr->prev;
		free(curr);
	}
	free(freq_list);
}
