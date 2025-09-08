#include "math_utils.h"
int add(int a, int b)
{
    return a + b;
}

int divide(int a, int b)
{
    return (int)a / b;
}

int power(int a, int b)
{
    int res = a;
    while (b > 0)
    {
        res *= a;
        b--;
    }
    return res;
}

int isEven(int a)
{
    return a % 2 == 0;
}