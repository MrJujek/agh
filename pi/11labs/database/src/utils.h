#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct S_GenericNode
{
    void *data;
    struct S_GenericNode *next;
} S_GenericNode;

typedef struct D_GenericNode
{
    void *data;
    struct D_GenericNode *next;
    struct D_GenericNode *prev;
} D_GenericNode;

#define NAME_MAX 50
#define LOCATION_MAX 100

typedef enum
{
    SALE,
    RENTAL,
    VISIT
} OfferType;

extern const char *offer_str[];

typedef struct
{
    unsigned int id;
    char name[NAME_MAX];
    int year;
    OfferType offer;
    float cost;
    char location[LOCATION_MAX];
} Record;

typedef struct RecordNode
{
    Record data;
    struct RecordNode *next;
} RecordNode;

void input_record(Record *r);
void print_record(const Record *r);
void edit_record_fields_interactive(Record *r);

RecordNode *create_record_node(Record data);

void append_record_to_list(RecordNode **head_ref, Record data);
RecordNode *insert_record_after_node(RecordNode *prev_node, Record data);

RecordNode *load_records_from_file(RecordNode **head_ref, RecordNode *insert_after_this_node, bool insert_mode, const char *filename);
void save_records_to_file(RecordNode *head, const char *filename);

RecordNode *delete_selected_node(RecordNode **head_ref, RecordNode *node_to_delete);

void free_record_list(RecordNode **head_ref);

#endif // UTILS_H