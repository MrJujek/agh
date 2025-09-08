#include <stdio.h>
#include "dzialania.h"

float numbers[100];

float akcja(tdzialanie choice, int start, int end, float number)
{
    printf("%d%d%f", start, end, number);
    switch (choice)
    {
    case td_zerowanie:
        for (int i = 0; i < 100; i++)
        {
            numbers[i] = 0;
        }
        break;

    case td_odczyt:
        return numbers[start];

    default:
        break;
    }

    return 0.0;
}