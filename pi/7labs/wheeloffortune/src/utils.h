#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_PLAYERS 4
#define MAX_NAME_LEN 50
#define MAX_PHRASE_LEN 100
#define NUM_PHRASES 15

char *getRandomPhrase(FILE *file);

void hide_phrase(const char *phrase, char *hidden);

void display_status(const char *hidden_phrase, char players[MAX_PLAYERS][MAX_NAME_LEN], int scores[], int num_players);

int update_hidden_phrase(const char *phrase, char *hidden_phrase, char guess);

int is_phrase_complete(const char *hidden_phrase);

#endif