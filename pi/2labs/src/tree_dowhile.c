#include <stdio.h>

void tree_dowhile(int height)
{
    int max_width = 1 + height * 2;
    int i = 0;
    do
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
    } while (i < height);

    i = 0;
    do
    {
        printf(" ");
        i++;
    } while (i <= (max_width / 2));
    printf("*\n");

    i = 0;
    do
    {
        printf(" ");
        i++;
    } while (i <= (max_width / 2) - 1);
    printf("***\n");
}
