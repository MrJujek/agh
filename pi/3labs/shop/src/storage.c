#include <stdio.h>
#include "storage.h"

goods tab[100] = {
    {"abc", 1.23, 23, sold},
    {"bcd", 2.34, 22, not_sold}};

void show_not_sold()
{
    for (int i = 0; i < 100; i++)
    {
        if (tab[i].status == not_sold)
        {
            printf("Name: %s\nPrice: %f\nVAT: %d", tab[i].name, tab[i].price, tab[i].VAT);
        }
    }
}