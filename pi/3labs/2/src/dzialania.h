#ifndef DZIALANIA_H
#define DZIALANIA_H

extern float numbers[100];

typedef enum
{
    td_zerowanie,
    td_odczyt
} tdzialanie;

float akcja(tdzialanie wybor, int start, int end, float number);

#endif