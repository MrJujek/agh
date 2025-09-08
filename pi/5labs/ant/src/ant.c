#include <stdio.h>
#include <stdlib.h>
#include "ant.h"

void initBoard(int ***board, int zeros)
{
    *board = (int **)malloc(SIZE * sizeof(int *));
    if (*board == NULL)
    {
        fprintf(stderr, "Error\n");
    }

    for (int i = 0; i < SIZE; i++)
    {
        (*board)[i] = (int *)malloc(SIZE * sizeof(int));
        if ((*board)[i] == NULL)
        {
            fprintf(stderr, "Error\n");
        }
    }

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (zeros == 0)
            {
                (*board)[i][j] = rand() % 8 + 1;
            }
            else
            {
                (*board)[i][j] = 0;
            }
        }
    }
}

void freeBoard(int **board)
{
    for (int i = 0; i < SIZE; i++)
    {
        free(board[i]);
    }
    free(board);
}

void displayBoard(int **board)
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void displayPath(int **path, Ant *ant, int startX, int startY)
{
    printf("\033[H");

    for (int i = 0; i < SIZE + 2; i++)
    {
        printf("\033[48;5;27m  \033[0m");
    }
    printf("\n");

    for (int i = 0; i < SIZE; i++)
    {
        printf("\033[48;5;27m  \033[0m");

        for (int j = 0; j < SIZE; j++)
        {
            if (i == ant->y && j == ant->x)
            {
                char arrow;
                switch (ant->dir)
                {
                case NORTH:
                    arrow = '^';
                    break;
                case EAST:
                    arrow = '>';
                    break;
                case SOUTH:
                    arrow = 'v';
                    break;
                case WEST:
                    arrow = '<';
                    break;
                }
                printf("\033[1m\033[48;5;196m%c \033[0m", arrow);
            }
            else if (j == startX && i == startY)
            {
                printf("\033[48;5;226m  \033[0m");
            }
            else if (path[i][j] == 1)
            {
                printf("\033[48;5;15m  \033[0m");
            }
            else
            {
                printf("\033[48;5;0m  \033[0m");
            }
        }

        printf("\033[48;5;27m  \033[0m\n");
    }

    for (int i = 0; i < SIZE + 2; i++)
    {
        printf("\033[48;5;27m  \033[0m");
    }
    printf("\n");

    fflush(stdout);
}

#ifndef COOL_MOVES
void makeMove(Ant *ant, direction dir)
{
    switch (dir)
    {
    case NORTH:
        ant->y = (ant->y + SIZE - 1) % SIZE;
        break;
    case EAST:
        ant->x = (ant->x + 1) % SIZE;
        break;
    case SOUTH:
        ant->y = (ant->y + 1) % SIZE;
        break;
    case WEST:
        ant->x = (ant->x + SIZE - 1) % SIZE;
        break;
    default:
        break;
    }
}
#endif

void moveAnt(Ant *ant, int state, int **board, int **path)
{
#ifdef COOL_MOVES
    switch (state)
    {
    case 1:
        board[ant->y][ant->x] = 7;
        ant->y = (ant->y + 1) % SIZE;
        break;
    case 2:
        board[ant->y][ant->x] = 4;
        ant->dir = (ant->dir + 1) % 4;
        break;
    case 3:
        board[ant->y][ant->x] = 2;
        ant->dir = (ant->dir + 3) % 4;
        break;
    case 4:
        board[ant->y][ant->x] = 6;
        ant->x = (ant->x + 1) % SIZE;
        break;
    case 5:
        board[ant->y][ant->x] = 3;
        ant->x = (ant->x + SIZE - 1) % SIZE;
        break;
    case 6:
        board[ant->y][ant->x] = 5;
        ant->y = (ant->y + SIZE - 1) % SIZE;
        break;
    case 7:
        board[ant->y][ant->x] = 8;
        ant->dir = (ant->dir + 2) % 4;
        break;
    case 8:
        board[ant->y][ant->x] = 1;
        break;
    }

    path[ant->y][ant->x] = 1;
#else
    switch (state)
    {
    case 1:
        board[ant->y][ant->x] = 7;
        makeMove(ant, ant->dir);
        break;
    case 2:
        board[ant->y][ant->x] = 4;
        ant->dir = (ant->dir + 1) % 4;
        break;
    case 3:
        board[ant->y][ant->x] = 2;
        ant->dir = (ant->dir + 3) % 4;
        break;
    case 4:
        board[ant->y][ant->x] = 6;
        makeMove(ant, (ant->dir + 1) % 4);
        break;
    case 5:
        board[ant->y][ant->x] = 3;
        makeMove(ant, (ant->dir + 3) % 4);
        break;
    case 6:
        board[ant->y][ant->x] = 5;
        makeMove(ant, (ant->dir + 2) % 4);
        break;
    case 7:
        board[ant->y][ant->x] = 8;
        ant->dir = (ant->dir + 2) % 4;
        break;
    case 8:
        board[ant->y][ant->x] = 1;
        break;
    }

    path[ant->y][ant->x] = (path[ant->y][ant->x] + 1) % 2;
#endif
}

void printAnt(Ant *ant)
{
    printf("%d %d %c\n", ant->x, ant->y,
           ant->dir == NORTH ? 'N' : ant->dir == EAST ? 'E'
                                 : ant->dir == SOUTH  ? 'S'
                                                      : 'W');
}

void antSimulation(int **board, Ant *ant, const int steps)
{
    int **path;
    initBoard(&path, 1);

    int startX = ant->x, startY = ant->y;
    int state;

    path[startY][startX] = 1;

    printf("\033[2J");

    for (int i = 0; i < steps; i++)
    {
        state = board[ant->y][ant->x];
        moveAnt(ant, state, board, path);
        displayPath(path, ant, startX, startY);
        usleep(100000);
    }

    freeBoard(path);
}