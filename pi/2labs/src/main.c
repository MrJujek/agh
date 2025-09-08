#include <stdio.h>
#include "tree_for.h"
#include "tree_while.h"
#include "tree_dowhile.h"

int main(void)
{
    int height;
    printf("Podaj wysokość choinki: ");
    scanf("%d", &height);
    printf("Wysokość choinki to % d\n", height);

    printf("For loop:\n");
    tree_for(height);

    printf("While loop:\n");
    tree_while(height);

    printf("Do while loop:\n");
    tree_dowhile(height);

    return 0;
}
