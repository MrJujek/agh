#pragma once

#define NAME_MAX 100
#define LOCATION_MAX 100

typedef enum
{
    SALE,
    RENTAL,
    VISIT
} OfferType;

typedef struct
{
    unsigned int id;
    char name[NAME_MAX];
    int year;
    OfferType offer;
    float cost;
    char location[LOCATION_MAX];
} Record;

void input_record(Record *r);

void save_record(FILE *file, Record *r);

void print_record(Record *r);

int read_record(FILE *file, int index, Record *r);

void search_by_name(FILE *file, const char *fragment);

void search_by_budget(FILE *file, float max_cost);

int change_cost(FILE *file, unsigned int id, float new_cost);

void average_cost(FILE *file);