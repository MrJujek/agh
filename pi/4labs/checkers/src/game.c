#include "checkers.h"
#include <stdio.h>

void startGame()
{
    Piece player = WHITE;
    int gameEnded = 0;
    Checkers game;
    initializeBoard(&game);

    char x1,
        x2;
    int y1, y2;

    int targetX[4], targetY[4], count;

    int restartTyping = 0;

    int captured = 0;

    int didntCapture = 0;

    int error = 0;

    int nextCaptured = 0;

    int piecesLeft[2] = {
        SIZE_W * PAWN_ROWS / 2,
        SIZE_W * PAWN_ROWS / 2,
    }; // BLACK / WHITE

    while (gameEnded == 0)
    {
        if (piecesLeft[0] == 0)
        {
            gameEnded = 1;
            displayBoard(&game);
            printf("White player wins!\n");
            break;
        }
        else if (piecesLeft[1] == 0)
        {
            gameEnded = 1;
            displayBoard(&game);
            printf("Black player wins!\n");
            break;
        }

        if (!canMove(&game, player))
        {
            gameEnded = 1;
            printf("No possbible moves. It is a DRAW!\n");
            break;
        }

        if (restartTyping == 0)
        {
            printf("\n%s player turn:\n", player == WHITE ? "White" : "Black");
            displayBoard(&game);
        }
        else
        {
            restartTyping = 0;
        }

        while (1)
        {
            printf("Type piece: ");
            scanf(" %c%d", &x1, &y1);
            while (getchar() != '\n')
            {
            };

            if (checkForPiece(&game, x1 - 'A', y1 - 1, player))
            {
                break;
            }
            else
            {
                printf("Type correct piece.\n");
            }
        }

        while (1)
        {
            printf("Type destination: ");
            scanf(" %c%d", &x2, &y2);
            while (getchar() != '\n')
            {
            };

            if (checkForPiece(&game, x2 - 'A', y2 - 1, EMPTY))
            {
                break;
            }
            else
            {
                printf("Type correct destination.\n");
            }
        }
        // printf("%c%d -> %c%d\n", x1, y1, x2, y2);

        while (captureOptions(&game, x1 - 'A', y1 - 1, player, targetX, targetY, &count))
        {
            // printf("\n---while\n");
            if (captured == 1)
            {
                // printf("-----next capture\n");
                displayBoard(&game);

                captured = 0;
                nextCaptured = 1;

                while (1)
                {
                    printf("Type next capture: ");
                    scanf(" %c%d", &x2, &y2);
                    while (getchar() != '\n')
                    {
                    };

                    if (checkForPiece(&game, x2 - 'A', y2 - 1, EMPTY))
                    {
                        break;
                    }
                    else
                    {
                        printf("Type correct destination.\n");
                    }
                }
            }
            else
            {
                captured = 0;
            }

            for (int i = 0; i < count; i++)
            {
                // printf("-----for\n");
                if (targetX[i] == x2 - 'A' && targetY[i] == y2 - 1)
                {
                    // printf("-------trying to capture\n");
                    if (!capturePawn(&game, x1 - 'A', y1 - 1, (x2 - 'A' + x1 - 'A') / 2, (y1 - 1 + y2 - 1) / 2, player))
                    {
                        // printf("---------error\n");
                        error = 1;
                        break;
                    }

                    piecesLeft[player == WHITE ? 0 : 1] -= 1;

                    nextCaptured = 0;
                    captured = 1;
                    x1 = x2;
                    y1 = y2;
                    break;
                }
            }

            if (error == 1)
            {
                break;
            }

            if (nextCaptured == 1)
            {
                captured = 1;
                continue;
            }

            if (captured == 0)
            {
                didntCapture = 1;
                restartTyping = 1;
                break;
            }
        }

        if (captured == 0 && canPlayerCapture(&game, player))
        {
            didntCapture = 1;
        }

        if (didntCapture == 1)
        {
            printf("You have to capture enemy pawn. Try again.\n");
            didntCapture = 0;
            continue;
        }

        if (captured == 1)
        {
            player = (player == WHITE ? BLACK : WHITE);
            captured = 0;
            continue;
        }

        if (error == 1 || !makeMove(&game, x1 - 'A', y1 - 1, x2 - 'A', y2 - 1, player))
        {
            printf("Wrong move. Try again.\n");
            restartTyping = 1;
            continue;
        }
        else
        {
            player = (player == WHITE ? BLACK : WHITE);
        }
    }
}