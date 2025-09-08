#ifndef CHECKERS_H
#define CHECKERS_H

#define SIZE_W 8
#define SIZE_H 8
#define PAWN_ROWS 2

typedef enum
{
    EMPTY,
    WHITE,
    BLACK
} Piece;

typedef struct
{
    Piece board[SIZE_W][SIZE_H];
} Checkers;

void initializeBoard(Checkers *game);

void displayBoard(Checkers *game);

int checkForPiece(Checkers *game, int x, int y, Piece player);

int canMove(Checkers *game, Piece player);

int makeMove(Checkers *game, int x1, int y1, int x2, int y2, Piece player);

int captureOptions(Checkers *game, int x, int y, Piece player, int targetX[], int targetY[], int *count);

int capturePawn(Checkers *game, int myX, int myY, int oppX, int oppY, Piece player);

int canPlayerCapture(Checkers *game, Piece player);

#endif