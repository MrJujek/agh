#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_PLAYERS 4
#define MAX_NAME_LEN 50
#define MAX_PHRASE_LEN 100
#define NUM_PHRASES 15

const char *phrases[NUM_PHRASES] = {
    "Hello World", "Programming in C", "Code Compilation",
    "Wheel of Fortune", "Standard Input", "Computer Science",
    "Binary Search", "Pointer Arithmetic", "Memory Allocation",
    "Dynamic Array", "Recursive Function", "Header File",
    "Syntax Error", "Operating System", "Random Access"};

void hide_phrase(const char *phrase, char *hidden)
{
    for (int i = 0; phrase[i]; i++)
    {
        if (isalpha(phrase[i]))
        {
            hidden[i] = '_';
        }
        else
        {
            hidden[i] = phrase[i];
        }
    }
    hidden[strlen(phrase)] = '\0';
}

void display_status(const char *hidden_phrase, char players[MAX_PLAYERS][MAX_NAME_LEN], int scores[], int num_players)
{
    printf("\nCurrent phrase: %s\n", hidden_phrase);
    for (int i = 0; i < num_players; i++)
    {
        printf("%s: %d points\n", players[i], scores[i]);
    }
    printf("\n");
}

int update_hidden_phrase(const char *phrase, char *hidden_phrase, char guess)
{
    int found = 0;
    for (int i = 0; phrase[i]; i++)
    {
        if (tolower(phrase[i]) == guess && hidden_phrase[i] == '_')
        {
            hidden_phrase[i] = phrase[i];
            found++;
        }
    }
    return found;
}

int is_phrase_complete(const char *hidden_phrase)
{
    for (int i = 0; hidden_phrase[i]; i++)
    {
        if (hidden_phrase[i] == '_')
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    srand(time(NULL));

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

    const char *selected_phrase = phrases[rand() % NUM_PHRASES];
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
