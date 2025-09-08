#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_WORDS 1000
#define MAX_LENGTH 100

int is_palindrome(const char *word)
{
    int len = strlen(word);
    for (int i = 0; i < len / 2; i++)
    {
        if (tolower(word[i]) != tolower(word[len - 1 - i]))
        {
            return 0;
        }
    }
    return 1;
}

void remove_punctuation_and_digits(char *word)
{
    char *p = word;
    while (*word)
    {
        if (isalpha(*word))
        {
            *p++ = *word;
        }
        word++;
    }
    *p = '\0';
}

void shuffle_words(char words[][MAX_LENGTH], int count)
{
    for (int i = 0; i < count; i++)
    {
        int random_index = rand() % count;
        char temp[MAX_LENGTH];
        strcpy(temp, words[i]);
        strcpy(words[i], words[random_index]);
        strcpy(words[random_index], temp);
    }
}

int main()
{
    char words[MAX_WORDS][MAX_LENGTH];
    int word_count = 0;
    char input[MAX_LENGTH];

    printf("Wpisuj słowa lub zdania. Wpisz '<STOP>', aby zakończyć wprowadzanie.\n");

    while (1)
    {
        printf("Wpisz coś: ");
        fgets(input, MAX_LENGTH, stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strstr(input, "<STOP>") != NULL)
        {
            break;
        }

        char *token = strtok(input, " ");
        while (token != NULL)
        {
            char cleaned_word[MAX_LENGTH];
            strcpy(cleaned_word, token);
            remove_punctuation_and_digits(cleaned_word);
            if (strlen(cleaned_word) > 0)
            {
                strcpy(words[word_count++], cleaned_word);
                if (is_palindrome(cleaned_word))
                {
                    printf("Wow, palindromy >> %s <<\n", cleaned_word);
                }
            }
            token = strtok(NULL, " ");
        }
    }

    srand(time(NULL));
    while (1)
    {
        shuffle_words(words, word_count);
        printf("Wygenerowane zdanie: ");
        for (int i = 0; i < word_count; i++)
        {
            printf("%s ", words[i]);
        }

        char punctuation[] = {'.', '?', '!'};
        printf("%c\n", punctuation[rand() % 3]);

        printf("Czy wygenerować następne zdanie? (tak/nie): ");
        fgets(input, MAX_LENGTH, stdin);
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "nie") == 0)
        {
            printf("Koniec pracy.\n");
            break;
        }
    }

    return 0;
}