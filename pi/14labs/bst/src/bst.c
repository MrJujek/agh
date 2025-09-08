#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bst.h"

Node *createNode(int key, const char *word)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        fprintf(stderr, "Error: Could not allocate memory for the node.\n");
        return NULL;
    }
    newNode->key = key;
    newNode->word = strdup(word);
    if (newNode->word == NULL)
    {
        fprintf(stderr, "Error: Could not allocate memory for the word.\n");
        free(newNode);
        return NULL;
    }
    newNode->parent = newNode->left = newNode->right = NULL;
    return newNode;
}

Node *insertNode(Node *root, Node *newNode)
{
    if (root == NULL)
    {
        return newNode;
    }

    Node *current = root;
    Node *parent = NULL;

    while (current != NULL)
    {
        parent = current;
        if (newNode->key < current->key)
        {
            current = current->left;
        }
        else if (newNode->key > current->key)
        {
            current = current->right;
        }
        else
        {
            printf("Key %d already exists. Node not added.\n", newNode->key);
            free(newNode->word);
            free(newNode);
            return root;
        }
    }

    newNode->parent = parent;
    if (newNode->key < parent->key)
    {
        parent->left = newNode;
    }
    else
    {
        parent->right = newNode;
    }

    return root;
}

void prettyPrint(Node *root, char *prefix, int isLeft)
{
    if (root == NULL)
    {
        if (prefix[0] == '\0')
            printf("The tree is empty.\n");
        return;
    }
    char newPrefix[512];
    if (root->right)
    {
        sprintf(newPrefix, "%s%s", prefix, isLeft ? "│   " : "    ");
        prettyPrint(root->right, newPrefix, 0);
    }
    printf("%s%s%d (%s)\n", prefix, isLeft ? "└── " : "┌── ", root->key, root->word);
    if (root->left)
    {
        sprintf(newPrefix, "%s%s", prefix, isLeft ? "    " : "│   ");
        prettyPrint(root->left, newPrefix, 1);
    }
}

Node *findMin(Node *root)
{
    if (root == NULL)
        return NULL;
    Node *current = root;
    while (current->left != NULL)
    {
        current = current->left;
    }
    return current;
}

Node *findMax(Node *root)
{
    if (root == NULL)
        return NULL;
    Node *current = root;
    while (current->right != NULL)
    {
        current = current->right;
    }
    return current;
}

Node *findNode(Node *root, int key)
{
    Node *current = root;
    while (current != NULL)
    {
        if (key < current->key)
        {
            current = current->left;
        }
        else if (key > current->key)
        {
            current = current->right;
        }
        else
        {
            return current;
        }
    }
    return NULL;
}

Node *findSuccessor(Node *node)
{
    if (node == NULL)
        return NULL;

    if (node->right != NULL)
    {
        return findMin(node->right);
    }

    Node *p = node->parent;
    Node *current = node;
    while (p != NULL && current == p->right)
    {
        current = p;
        p = p->parent;
    }
    return p;
}

Node *findPredecessor(Node *node)
{
    if (node == NULL)
        return NULL;

    if (node->left != NULL)
    {
        return findMax(node->left);
    }

    Node *p = node->parent;
    Node *current = node;
    while (p != NULL && current == p->left)
    {
        current = p;
        p = p->parent;
    }
    return p;
}

void freeTree(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root->word);
    free(root);
}

int isNumber(const char *str)
{
    if (str == NULL || *str == '\0' || isspace((unsigned char)*str))
    {
        return 0;
    }
    char *end;
    strtol(str, &end, 10);
    while (isspace((unsigned char)*end))
    {
        end++;
    }
    return *end == '\0';
}

static void swap(Node **a, Node **b)
{
    Node *t = *a;
    *a = *b;
    *b = t;
}

static void bubbleSort(Node *nodes[], int n)
{
    int i, j;
    int swapped;
    for (i = 0; i < n - 1; i++)
    {
        swapped = 0;
        for (j = 0; j < n - i - 1; j++)
        {
            if (nodes[j]->key > nodes[j + 1]->key)
            {
                swap(&nodes[j], &nodes[j + 1]);
                swapped = 1;
            }
        }
        if (swapped == 0)
        {
            break;
        }
    }
}

static int countNodes(Node *root)
{
    if (root == NULL)
    {
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}

static void storeNodesUnordered(Node *node, Node *nodes[], int *index_ptr)
{
    if (node == NULL)
    {
        return;
    }
    nodes[*index_ptr] = node;
    (*index_ptr)++;
    storeNodesUnordered(node->left, nodes, index_ptr);
    storeNodesUnordered(node->right, nodes, index_ptr);
}

static Node *buildBalancedTreeFromSortedArray(Node *nodes[], int start, int end)
{
    if (start > end)
    {
        return NULL;
    }
    int mid = start + (end - start) / 2;
    Node *node = nodes[mid];
    node->parent = NULL;
    node->left = buildBalancedTreeFromSortedArray(nodes, start, mid - 1);
    if (node->left)
    {
        node->left->parent = node;
    }
    node->right = buildBalancedTreeFromSortedArray(nodes, mid + 1, end);
    if (node->right)
    {
        node->right->parent = node;
    }
    return node;
}

Node *balanceTree(Node *root)
{
    if (root == NULL)
    {
        return NULL;
    }

    int n = countNodes(root);
    if (n <= 2)
    {
        return root;
    }

    Node **nodes = (Node **)malloc(n * sizeof(Node *));
    if (nodes == NULL)
    {
        fprintf(stderr, "Error: Could not allocate memory for balancing.\n");
        return root;
    }

    int index = 0;
    storeNodesUnordered(root, nodes, &index);

    bubbleSort(nodes, n);

    Node *newRoot = buildBalancedTreeFromSortedArray(nodes, 0, n - 1);

    free(nodes);
    return newRoot;
}