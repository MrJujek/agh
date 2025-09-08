#include <stdio.h>

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void sort(int *a, int *b, int *c)
{
    if (*a > *b)
    {
        swap(a, b);
    }
    if (*a > *c)
    {
        swap(a, c);
    }
    if (*b > *c)
    {
        swap(b, c);
    }
}

int main(void)
{
    int a = 5;
    int b = 10;
    int c = 0;
    printf("a: %d, b: %d, c: %d\n", a, b, c);
    sort(&a, &b, &c);
    printf("a: %d, b: %d, c: %d\n", a, b, c);
}
