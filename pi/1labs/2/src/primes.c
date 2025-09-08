#include <math.h>
#include <stdio.h>
#include "primes.h"

void primes()
{
    int isPrime;
    for (int i = 2; i <= 1000; i++)
    {
        isPrime = 1;
        for (int j = 2; j < (int)sqrt(i); j++)
        {
            int rest = i % j;
            if (rest == 0)
            {
                isPrime = 0;
                break;
            }
        }
        if (isPrime == 1)
        {
            printf("%d\n", i);
        }
    }
}