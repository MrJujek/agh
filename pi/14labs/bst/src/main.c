#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bst.h"

int main()
{
    Node *root = NULL;
    char line[256];
    char command[50];
    char value_str[200];
    int value;

    while (1)
    {
        printf("\n> ");
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            break;
        }

        line[strcspn(line, "\n")] = 0;

        int items_scanned = sscanf(line, "%49s %199[^\n]", command, value_str);

        if (items_scanned <= 0)
        {
            continue;
        }

        if (isNumber(command))
        {
            if (items_scanned != 2)
            {
                fprintf(stderr, "Error: A word must follow a number.\n");
                continue;
            }
            value = atoi(command);
            Node *newNode = createNode(value, value_str);
            if (newNode != NULL)
            {
                root = insertNode(root, newNode);
                prettyPrint(root, "", 1);
            }
        }
        else
        {
            for (int i = 0; command[i]; i++)
            {
                command[i] = toupper(command[i]);
            }

            if (strcmp(command, "SHOW") == 0)
            {
                prettyPrint(root, "", 1);
            }
            else if (strcmp(command, "MIN") == 0)
            {
                Node *minNode = findMin(root);
                if (minNode)
                {
                    printf("Minimum value: (%d, %s)\n", minNode->key, minNode->word);
                }
                else
                {
                    printf("The tree is empty.\n");
                }
            }
            else if (strcmp(command, "MAX") == 0)
            {
                Node *maxNode = findMax(root);
                if (maxNode)
                {
                    printf("Maximum value: (%d, %s)\n", maxNode->key, maxNode->word);
                }
                else
                {
                    printf("The tree is empty.\n");
                }
            }
            else if (strcmp(command, "FIND") == 0)
            {
                if (items_scanned != 2 || !isNumber(value_str))
                {
                    fprintf(stderr, "Error: The FIND command requires a numeric value.\n");
                    continue;
                }
                value = atoi(value_str);
                Node *foundNode = findNode(root, value);
                if (foundNode)
                {
                    printf("Node found: (%d, %s)\n", foundNode->key, foundNode->word);
                }
                else
                {
                    printf("Node with key %d not found.\n", value);
                }
            }
            else if (strcmp(command, "NEXT") == 0)
            {
                if (items_scanned != 2 || !isNumber(value_str))
                {
                    fprintf(stderr, "Error: The NEXT command requires a numeric value.\n");
                    continue;
                }
                value = atoi(value_str);
                Node *node = findNode(root, value);
                if (node)
                {
                    Node *successor = findSuccessor(node);
                    if (successor)
                    {
                        printf("Successor of node %d is: (%d, %s)\n", value, successor->key, successor->word);
                    }
                    else
                    {
                        printf("Node with key %d has no successor (it is the largest element).\n", value);
                    }
                }
                else
                {
                    printf("Node with key %d not found.\n", value);
                }
            }
            else if (strcmp(command, "PREV") == 0)
            {
                if (items_scanned != 2 || !isNumber(value_str))
                {
                    fprintf(stderr, "Error: The PREV command requires a numeric value.\n");
                    continue;
                }
                value = atoi(value_str);
                Node *node = findNode(root, value);
                if (node)
                {
                    Node *predecessor = findPredecessor(node);
                    if (predecessor)
                    {
                        printf("Predecessor of node %d is: (%d, %s)\n", value, predecessor->key, predecessor->word);
                    }
                    else
                    {
                        printf("Node with key %d has no predecessor (it is the smallest element).\n", value);
                    }
                }
                else
                {
                    printf("Node with key %d not found.\n", value);
                }
            }
            else if (strcmp(command, "BALANCE") == 0)
            {
                root = balanceTree(root);
                prettyPrint(root, "", 1);
            }
            else if (strcmp(command, "EXIT") == 0)
            {
                break;
            }
            else
            {
                fprintf(stderr, "Error: Unrecognized command '%s'.\n", command);
            }
        }
    }

    freeTree(root);
    return 0;
}