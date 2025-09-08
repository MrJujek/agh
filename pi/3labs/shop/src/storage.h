#ifndef TABLICALICZB_H
#define TABLICALICZB_H

typedef enum
{
    sold,
    not_sold
} status;

typedef struct
{
    char name[10];
    float price;
    int VAT;
    status status;
} goods;

extern goods tab[100];

void show_not_sold();

#endif