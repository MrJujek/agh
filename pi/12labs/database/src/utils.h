#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum
{
    SALE,
    RENTAL,
    VISIT
} OfferType;

typedef struct
{
    unsigned int id;
    char name[50];
    int year;
    OfferType offer;
    float cost;
    char location[100];
} Record;

typedef struct RecordNode
{
    Record data;
    struct RecordNode *next;
} RecordNode;

typedef struct SortedNode
{
    struct RecordNode *record_node_ptr;
    struct SortedNode *next;
    struct SortedNode *prev;
} SortedNode;

typedef enum
{
    BY_ID,
    BY_NAME,
    BY_COST
} SortCriteria;

extern const char *offer_str[];

void input_record(Record *r);
void print_record(const Record *r);

RecordNode *append_to_main_list(RecordNode **head_ref, Record data);
void delete_from_main_list(RecordNode **head_ref, RecordNode *node_to_delete);

void insert_sorted(SortedNode **head_ref, RecordNode *record_node, SortCriteria criteria);
void delete_from_sorted_list(SortedNode **head_ref, RecordNode *record_node_to_delete);

void load_data_and_build_lists(RecordNode **main_head,
                               SortedNode **id_head,
                               SortedNode **name_head,
                               SortedNode **cost_head,
                               const char *filename);
void save_records_to_file(RecordNode *head, const char *filename);

void free_all_lists(RecordNode **main_head, SortedNode **id_head, SortedNode **name_head, SortedNode **cost_head);

#endif