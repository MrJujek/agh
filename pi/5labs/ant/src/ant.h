#ifndef ANT_H
#define ANT_H

#define SIZE 51

#define COOL_MOVES

typedef enum
{
    NORTH,
    EAST,
    SOUTH,
    WEST
} direction;

typedef struct
{
    int x, y;
    direction dir;
} Ant;

void initBoard(int ***board, int zeros);

void freeBoard(int **board);

void displayBoard(int **board);

void displayPath(int **path, Ant *ant, int startX, int startY);

void makeMove(Ant *ant, direction dir);

void moveAnt(Ant *ant, int state, int **board, int **path);

void printAnt(Ant *ant);

void antSimulation(int **board, Ant *ant, const int steps);

#endif