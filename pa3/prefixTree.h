#include <stdlib.h>


typedef struct treeRoot{
   struct Node  *root;
   struct Node *ptr;
} treeRoot;

typedef struct Node{
    char letter;
    struct Node** branches;
    struct FreqList *freak;
    char isWord;
} Node;



treeRoot *treeInit();

void insertNode(treeRoot *tree, char c);



