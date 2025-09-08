#include "utils.h"

char *getRandomPhrase(FILE *file)
{
    char **lines = NULL;
    size_t lineCount = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file))
    {
        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
        {
            buffer[len - 1] = '\0';
            if (len > 1 && (buffer[len - 2] == '\n' || buffer[len - 2] == '\r'))
            {
                buffer[len - 2] = '\0';
            }
        }

        char **temp = realloc(lines, (lineCount + 1) * sizeof(char *));
        if (!temp)
        {
            perror("Error reallocating memory for lines");
            for (size_t i = 0; i < lineCount; i++)
            {
                free(lines[i]);
            }
            free(lines);
            fclose(file);
            return NULL;
        }
        lines = temp;

        lines[lineCount] = malloc(strlen(buffer) + 1);
        if (!lines[lineCount])
        {
            perror("Error allocating memory for line");
            for (size_t i = 0; i < lineCount; i++)
            {
                free(lines[i]);
            }
            free(lines);
            fclose(file);
            return NULL;
        }
        strcpy(lines[lineCount], buffer);
        lineCount++;
    }

    fclose(file);

    if (lineCount == 0)
    {
        fprintf(stderr, "File is empty or could not be read\n");
        return NULL;
    }

    size_t randomIndex = rand() % lineCount;
    char *randomLine = strdup(lines[randomIndex]);

    for (size_t i = 0; i < lineCount; i++)
    {
        free(lines[i]);
    }
    free(lines);

    return randomLine;
}

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
    char lower_guess = tolower(guess);

    for (int i = 0; phrase[i]; i++)
    {
        if (tolower(phrase[i]) == lower_guess && hidden_phrase[i] == '_')
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