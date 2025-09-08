#include <stdio.h>

typedef enum
{
    S0,
    S1
} status;

typedef enum
{
    Z0,
    Z1
} symbols;

#define MEMORY_SIZE 100
symbols memory[MEMORY_SIZE];

int poz = 0;

status state = S0;

void step()
{
    switch (state)
    {
    case S0:
        if (memory[poz] == Z0)
        {
            memory[poz] = Z1;
            state = S1;
            poz = poz < 99 ? poz + 1 : 0;
        }
        else
        {
            memory[poz] = Z0;
            state = S1;
            poz = poz > 0 ? poz - 1 : 99;
        }
        break;

    case S1:
        if (memory[poz] == Z0)
        {
            memory[poz] = Z1;
            state = S0;
            poz = poz > 0 ? poz - 1 : 99;
        }
        else
        {
            memory[poz] = Z0;
            state = S0;
        }
        break;
    }
}

int main()
{
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        memory[i] = Z0;
    }

    do
    {
        step();

        for (int i = 0; i < MEMORY_SIZE; i++)
        {
            printf("%c", memory[i] == Z0 ? '0' : '1');
        }
        printf("\n");
    } while (state != S0 || memory[poz] != Z0);

    return 0;
}