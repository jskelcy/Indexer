#include <stdlid.h>

typedef struct treeRoot(){
   struct Node  *root;
} treeRoot;

typedef struct Node(char letter,){
    char letter = letter;
    Node **branches;
    void *freak;
    char isWord;
} Node;

treeRoot* treeInit(){
    treeRoot *returnTree = (treeRoot *) malloc (sizeof(treeRoot));
    returnTree->root = (Node *) calloc(1,sizeof(Node));
    returnTree->root->branches = (Node **) calloc (36, sizeof (Node *));
    return returnTree;
}

Node* nodeInit(char letter,char isWord){
    Node *returnNode = (Node *) calloc(1,sizeof(Node));
    returnNode->letter= letter;
    returnNode->branches= (Node **) calloc (36, sizeof (Node *));
}

int hash(char c){

}

