#ifndef BST_H
#define BST_H

typedef struct Node {
    int key;
    char *word;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
} Node;

Node* createNode(int key, const char *word);

Node* insertNode(Node *root, Node *newNode);

void prettyPrint(Node *root, char *prefix, int isLeft);

Node* findMin(Node *root);

Node* findMax(Node *root);

Node* findNode(Node *root, int key);

Node* findSuccessor(Node *node);

Node* findPredecessor(Node *node);

void freeTree(Node *root);

int isNumber(const char *str);

Node *balanceTree(Node *root);

#endif