#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

const char *offer_str[] = {"Sale", "Rental", "Visit"};

void input_record(Record *r)
{
    printf("Enter ID (hexadecimal, e.g. 1A3F): 0x");
    scanf("%x", &r->id);
    getchar();

    printf("Enter title: ");
    fgets(r->name, NAME_MAX, stdin);
    r->name[strcspn(r->name, "\n")] = '\0';

    printf("Enter year: ");
    scanf("%d", &r->year);

    printf("Offer type (0 - Sale, 1 - Rental, 2 - Visit): ");
    int type;
    scanf("%d", &type);
    r->offer = (OfferType)type;

    printf("Enter cost (PLN): ");
    scanf("%f", &r->cost);
    getchar();

    printf("Enter location: ");
    fgets(r->location, LOCATION_MAX, stdin);
    r->location[strcspn(r->location, "\n")] = '\0';
}

void save_record(FILE *file, Record *r)
{
    fseek(file, 0, SEEK_END);
    fwrite(r, sizeof(Record), 1, file);
}

void print_record(Record *r)
{
    printf("ID: 0x%X\nName: %s\nYear: %d\nOffer: %s\nCost: %.2f PLN\nLocation: %s\n\n",
           r->id, r->name, r->year, offer_str[r->offer], r->cost, r->location);
}

int read_record(FILE *file, int index, Record *r)
{
    fseek(file, index * sizeof(Record), SEEK_SET);
    return fread(r, sizeof(Record), 1, file);
}

void search_by_name(FILE *file, const char *fragment)
{
    Record r;
    rewind(file);
    while (fread(&r, sizeof(Record), 1, file))
    {
        if (strstr(r.name, fragment))
        {
            print_record(&r);
        }
    }
}

void search_by_budget(FILE *file, float max_cost)
{
    Record r;
    rewind(file);
    while (fread(&r, sizeof(Record), 1, file))
    {
        if (r.cost <= max_cost)
        {
            print_record(&r);
        }
    }
}

int change_cost(FILE *file, unsigned int id, float new_cost)
{
    Record r;
    rewind(file);
    int index = 0;
    while (fread(&r, sizeof(Record), 1, file))
    {
        if (r.id == id)
        {
            r.cost = new_cost;
            fseek(file, index * sizeof(Record), SEEK_SET);
            fwrite(&r, sizeof(Record), 1, file);
            return 1;
        }
        index++;
    }
    return 0;
}

void average_cost(FILE *file)
{
    Record r;
    int count = 0;
    float sum = 0.0;
    rewind(file);
    while (fread(&r, sizeof(Record), 1, file))
    {
        sum += r.cost;
        count++;
    }
    if (count > 0)
    {
        printf("Average cost: %.2f PLN\n", sum / count);
    }
    else
    {
        printf("No records found.\n");
    }
}