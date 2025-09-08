#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void tree_for(int height)
{
    srand(time(0));

    int max_width = 1 + height * 2;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j <= (max_width / 2) - i; j++)
        {
            printf(" ");
        }
        for (int j = 0; j <= i * 2; j++)
        {
            if (j != 0 && j != i * 2 && rand() % 100 < 10)
            {
                printf("0");
            }
            else
            {
                printf("*");
            }
        }
        printf("\n");
    }
    for (int j = 0; j <= (max_width / 2); j++)
    {
        printf(" ");
    }
    printf("*\n");
    for (int j = 0; j <= (max_width / 2) - 1; j++)
    {
        printf(" ");
    }
    printf("***\n");
}
