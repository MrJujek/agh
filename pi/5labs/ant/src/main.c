#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "ant.h"

// #define USER_INPUT

int main()
{
#ifdef USER_INPUT
    unsigned seed;
    int steps;
    Ant ant = {0, 0, EAST};
    scanf("%u %d", &seed, &steps);
    srand(seed);
#else
    const int steps = 1000;
    srand(time(NULL));
    Ant ant = {SIZE / 2, SIZE / 2, NORTH};
#endif

    int **board;
    initBoard(&board, 0);

    antSimulation(board, &ant, steps);

#ifdef USER_INPUT
    displayBoard(board);
    printAnt(&ant);
#endif

    freeBoard(board);

    return 0;
}