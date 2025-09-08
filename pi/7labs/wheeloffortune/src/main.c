#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"

const char *phrases;

int main()
{
    srand(time(NULL));

    FILE *file = fopen("files/phrases.txt", "r");
    if (!file)
    {
        printf("No file with phrases found.");
        return -1;
    }

    char *phrase = getRandomPhrase(file);

    if (!phrase)
    {
        printf("Error while getting phrase.");
        return -1;
    }

    int num_players;
    char players[MAX_PLAYERS][MAX_NAME_LEN];
    int scores[MAX_PLAYERS] = {0};

    do
    {
        printf("Enter number of players (1-4): ");
        if (scanf("%d", &num_players) != 1)
        {
            while (getchar() != '\n')
                ;
            num_players = 0;
        }
        else
        {
            while (getchar() != '\n')
                ;
        }

        if (num_players < 1 || num_players > MAX_PLAYERS)
        {
            printf("Invalid number of players. Please enter a number between 1 and 4.\n");
        }
    } while (num_players < 1 || num_players > MAX_PLAYERS);

    for (int i = 0; i < num_players; i++)
    {
        printf("Enter name for player %d: ", i + 1);
        fgets(players[i], MAX_NAME_LEN, stdin);
        players[i][strcspn(players[i], "\n")] = '\0';
    }

    const char *selected_phrase = phrase;
    char hidden_phrase[MAX_PHRASE_LEN];
    hide_phrase(selected_phrase, hidden_phrase);

    int current_player = 0;
    while (!is_phrase_complete(hidden_phrase))
    {
        display_status(hidden_phrase, players, scores, num_players);

        printf("%s, guess a letter: ", players[current_player]);
        char input[10];
        fgets(input, sizeof(input), stdin);

        char guess = tolower(input[0]);

        if (!isalpha(guess))
        {
            printf("Invalid input. Please enter a letter.\n");
            continue;
        }

        int found = update_hidden_phrase(selected_phrase, hidden_phrase, guess);
        if (found > 0)
        {
            scores[current_player] += found;
            printf("Correct! The letter '%c' appears %d time(s).\n", guess, found);
        }
        else
        {
            printf("Sorry, no '%c' found.\n", guess);
            current_player = (current_player + 1) % num_players;
        }
    }

    printf("\nCongratulations! The full phrase was: \"%s\"\n", selected_phrase);
    display_status(hidden_phrase, players, scores, num_players);

    return 0;
}
