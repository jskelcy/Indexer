#include "FreqList.h"

FreqList *FLCreate() {
    FreqList *freq_list = malloc(sizeof(FreqList));
	FreqNode *freq_node = malloc(sizeof(FreqNode));
    freq_node->freq = 1;
    freq_node->prev = NULL;
    freq_node->next = NULL;
    freq_node->child = NULL;
    freq_list->root = freq_node;
    freq_list->current = NULL;
    return freq_list;
}

void FLInsert(FreqList *freq_list, char *filename) {
    FileNode *file_node;
	if (freq_list == NULL) {
		return;
	}
    if (filename == NULL || filename[0] == '\0') {
        return;
    }
	file_node = freq_list->current;
    if (file_node == NULL || filename != file_node->name) {
        /* Push new node */
    	file_node = malloc(sizeof(FileNode));
        file_node->name = filename;
        file_node->parent = freq_list->root;
		file_node->prev = NULL;
        file_node->next = freq_list->root->child;
        if (file_node->next != NULL) {
            file_node->next->prev = file_node;
        }
        freq_list->root->child = file_node;
        freq_list->current = file_node;
    } else {
		FreqNode *prev = file_node->parent, *next = prev->next, *curr = next;
        /* Free from previous list */
        if (file_node->next != NULL) {
            file_node->next->prev = file_node->prev;
        }
        if (file_node->prev != NULL) {
            file_node->prev->next = file_node->next;
        } else {
            prev->child = file_node->next;
        }
        file_node->prev = NULL;
        file_node->next = NULL;
        /* Create a new current node if not the correct one */
        if (next == NULL || next->freq != prev->freq + 1) {
			curr = malloc(sizeof(FreqNode));
            curr->freq = prev->freq + 1;
            curr->prev = prev;
            prev->next = curr;
			curr->next = next;
			if (next != NULL) {
				next->prev = curr;
			}
			curr->child = NULL;
        }
        file_node->parent = curr;
		/* Destroy the old node if there is nothing left */
		if (prev->prev != NULL && prev->child == NULL) {
			prev->prev->next = prev->next;
			if (prev->next != NULL) {
				prev->next->prev = prev->prev;
			}
			free(prev);
		}
		/* Insert into new list */
        file_node->next = curr->child;
        if (file_node->next != NULL) {
            file_node->next->prev = file_node;
		}
        curr->child = file_node;
    }
}

void FLPrint(FreqList *freq_list, FILE *fp) {
    FreqNode *curr = NULL, *front;
	int top5 = 0;
	if (freq_list == NULL || fp == NULL) {
		return;
	}
    for (front = freq_list->root; front != NULL; front = front->next) {
        curr = front;
    }
    for (; curr != NULL; curr = curr->prev) {
        FileNode *ptr;
        for (ptr = curr->child; ptr != NULL; ptr = ptr->next) {
	       	fprintf(fp, " %s %d", ptr->name, curr->freq);
			top5++;
			if (top5 == 5) {
				break;
			}
        }
		if (top5 == 5) {
			break;
		}
    }
	fprintf(fp, "\n");
}

void FLDestroy(FreqList *freq_list) {
    FreqNode *curr = NULL, *front;
	if (freq_list == NULL) {
		return;
	}
	/* free backwards first DLL */
    for (front = freq_list->root; front != NULL; front = front->next) {
        curr = front;
    }
    for (; curr != NULL; curr = front) {
        FileNode *ptr, *prev = NULL;
		/* free backwards second DLL */
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
