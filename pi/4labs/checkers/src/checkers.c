#include <stdio.h>
#include "checkers.h"

void initializeBoard(Checkers *game)
{
    for (int i = 0; i < SIZE_H; i++)
    {
        for (int j = 0; j < SIZE_W; j++)
        {
            if ((i + j) % 2 == 1)
            {
                if (i < PAWN_ROWS)
                {
                    (*game).board[i][j] = BLACK;
                }
                else if (i >= SIZE_H - PAWN_ROWS)
                {
                    (*game).board[i][j] = WHITE;
                }
                else
                {
                    (*game).board[i][j] = EMPTY;
                }
            }
            else
            {
                (*game).board[i][j] = EMPTY;
            }
        }
    }
}

void displayBoard(Checkers *game)
{
    printf(" \t");
    for (int j = 0; j < SIZE_W; j++)
    {
        printf(" %c", 'A' + j);
    }
    printf("\n\n");

    for (int i = 0; i < SIZE_H; i++)
    {
        printf("%d\t", i + 1);
        for (int j = 0; j < SIZE_W; j++)
        {
            switch ((*game).board[i][j])
            {
            case EMPTY:
                printf("%s", " .");
                break;
            case WHITE:
                printf("%s", " W");
                break;
            case BLACK:
                printf("%s", " B");
                break;
            default:
                printf("  ");
                break;
            }
        }
        printf("\t%d\n", i + 1);
    }

    printf("\n \t");
    for (int j = 0; j < SIZE_W; j++)
    {
        printf(" %c", 'A' + j);
    }
    printf("\n");
}

int checkForPiece(Checkers *game, int x, int y, Piece player)
{
    // printf("%s\n", ((*game).board[y][x] == EMPTY) ? "EMPTY\n" : ((*game).board[y][x] == WHITE) ? "WHITE\n"
    //                                                                                            : "BLACK\n");

    if (y >= 0 && y < SIZE_H && x >= 0 && x < SIZE_W && (*game).board[y][x] == player)
    {
        return 1;
    }
    return 0;
}

int canMove(Checkers *game, Piece player)
{
    for (int i = 0; i < SIZE_H; i++)
    {
        for (int j = 0; j < SIZE_W; j++)
        {
            if ((*game).board[i][j] == player)
            {
                int direction = (player == WHITE) ? -1 : 1;

                if (i + direction >= 0 && i + direction < SIZE_H)
                {
                    if (j - 1 >= 0 && (*game).board[i + direction][j - 1] == EMPTY)
                        return 1;
                    if (j + 1 < SIZE_W && (*game).board[i + direction][j + 1] == EMPTY)
                        return 1;
                }
            }
        }
    }

    return 0;
}

int makeMove(Checkers *game, int x1, int y1, int x2, int y2, Piece player)
{
    if (x2 < 0 || x2 >= SIZE_W || y2 < 0 || y2 >= SIZE_H)
    {
        return 0;
    }

    if ((*game).board[y2][x2] != EMPTY)
    {
        return 0;
    }

    if (!checkForPiece(game, x1, y1, player))
    {
        return 0;
    }

    int dx = x2 - x1, dy = y2 - y1;

    if ((dx == 1 || dx == -1) && (dy == 1 || dy == -1))
    {
        (*game).board[y2][x2] = (*game).board[y1][x1];
        (*game).board[y1][x1] = EMPTY;
        return 1;
    }

    return 0;
}

int canCapture(Checkers *game, Piece player)
{
    Piece opponent = (player == WHITE) ? BLACK : WHITE;
    int direction = (player == WHITE) ? -1 : 1;

    for (int i = 0; i < SIZE_H; i++)
    {
        for (int j = 0; j < SIZE_W; j++)
        {
            if ((*game).board[i][j] == player)
            {
                if (i + 2 * direction >= 0 && i + 2 * direction < SIZE_H &&
                    j - 2 >= 0 &&
                    (*game).board[i + direction][j - 1] == opponent &&
                    (*game).board[i + 2 * direction][j - 2] == EMPTY)
                {
                    return 1;
                }

                if (i + 2 * direction >= 0 && i + 2 * direction < SIZE_H &&
                    j + 2 < SIZE_W &&
                    (*game).board[i + direction][j + 1] == opponent &&
                    (*game).board[i + 2 * direction][j + 2] == EMPTY)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

int captureOptions(Checkers *game, int x, int y, Piece player, int targetX[], int targetY[], int *count)
{
    Piece opponent = (player == WHITE) ? BLACK : WHITE;
    int directions[] = {-1, 1};
    *count = 0;

    for (int d = 0; d < 2; d++)
    {
        int direction = directions[d];

        if (x - 2 >= 0 && y + 2 * direction >= 0 && y + 2 * direction < SIZE_H &&
            (*game).board[y + direction][x - 1] == opponent &&
            (*game).board[y + 2 * direction][x - 2] == EMPTY)
        {
            targetX[*count] = x - 2;
            targetY[*count] = y + 2 * direction;
            (*count)++;
        }

        if (x + 2 < SIZE_W && y + 2 * direction >= 0 && y + 2 * direction < SIZE_H &&
            (*game).board[y + direction][x + 1] == opponent &&
            (*game).board[y + 2 * direction][x + 2] == EMPTY)
        {
            targetX[*count] = x + 2;
            targetY[*count] = y + 2 * direction;
            (*count)++;
        }
    }

    return *count > 0;
}

int capturePawn(Checkers *game, int myX, int myY, int oppX, int oppY, Piece player)
{
    // printf("My coords:\t%d %d\n", myX, myY);
    // printf("Opp coords:\t%d %d\n", oppX, oppY);
    int targetX = myX + 2 * (oppX - myX);
    int targetY = myY + 2 * (oppY - myY);
    // printf("%d %d\n", targetX, targetY);

    if (targetX < 0 || targetX >= SIZE_W || targetY < 0 || targetY >= SIZE_H)
    {
        return 0;
    }

    Piece opponent = (player == WHITE) ? BLACK : WHITE;
    if ((*game).board[oppY][oppX] != opponent)
    {
        return 0;
    }

    if ((*game).board[targetY][targetX] != EMPTY)
    {
        return 0;
    }

    (*game).board[myY][myX] = EMPTY;
    (*game).board[oppY][oppX] = EMPTY;
    (*game).board[targetY][targetX] = player;

    return 1;
}

int canPlayerCapture(Checkers *game, Piece player)
{
    Piece opponent = (player == WHITE) ? BLACK : WHITE;
    int directions[] = {-1, 1};

    for (int y = 0; y < SIZE_H; y++)
    {
        for (int x = 0; x < SIZE_W; x++)
        {

            if ((*game).board[y][x] == player)
            {
                for (int d = 0; d < 2; d++)
                {
                    int direction = directions[d];

                    if (x - 2 >= 0 && y + 2 * direction >= 0 && y + 2 * direction < SIZE_H &&
                        (*game).board[y + direction][x - 1] == opponent &&
                        (*game).board[y + 2 * direction][x - 2] == EMPTY)
                    {
                        return 1;
                    }

                    if (x + 2 < SIZE_W && y + 2 * direction >= 0 && y + 2 * direction < SIZE_H &&
                        (*game).board[y + direction][x + 1] == opponent &&
                        (*game).board[y + 2 * direction][x + 2] == EMPTY)
                    {
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}