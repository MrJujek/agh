#include <stdio.h>

void tree_while(int height)
{
    int max_width = 1 + height * 2;
    int i = 0;
    while (i < height)
    {
        int j = 0;
        while (j <= (max_width / 2) - i)
        {
            printf(" ");
            j++;
        }

        j = 0;
        while (j <= i * 2)
        {
            printf("*");
            j++;
        }
        printf("\n");
        i++;
    }

    i = 0;
    while (i <= (max_width / 2))
    {
        printf(" ");
        i++;
    }
    printf("*\n");

    i = 0;
    while (i <= (max_width / 2) - 1)
    {
        printf(" ");
        i++;
    }
    printf("***\n");
}
