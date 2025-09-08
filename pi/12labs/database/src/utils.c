#include "utils.h"
#include <string.h>

const char *offer_str[] = {"Sale", "Rental", "Visit"};

void input_record(Record *r)
{
    printf("Enter ID (hex): 0x");
    scanf("%x", &r->id);
    while (getchar() != '\n')
        ;
    printf("Enter title: ");
    fgets(r->name, 50, stdin);
    r->name[strcspn(r->name, "\n")] = '\0';
    printf("Enter year: ");
    scanf("%d", &r->year);
    while (getchar() != '\n')
        ;
    printf("Offer type (0-Sale, 1-Rental, 2-Visit): ");
    int type;
    scanf("%d", &type);
    while (getchar() != '\n')
        ;
    r->offer = (type >= SALE && type <= VISIT) ? (OfferType)type : SALE;
    printf("Enter cost (PLN): ");
    scanf("%f", &r->cost);
    while (getchar() != '\n')
        ;
    printf("Enter location: ");
    fgets(r->location, 100, stdin);
    r->location[strcspn(r->location, "\n")] = '\0';
}

void print_record(const Record *r)
{
    if (!r)
    {
        printf("No record to display.\n");
        return;
    }
    printf("ID: 0x%X | Name: %s | Year: %d | Offer: %s | Cost: %.2f PLN | Location: %s\n",
           r->id, r->name, r->year, offer_str[r->offer], r->cost, r->location);
}

RecordNode *append_to_main_list(RecordNode **head_ref, Record data)
{
    RecordNode *newNode = (RecordNode *)malloc(sizeof(RecordNode));
    if (!newNode)
        exit(EXIT_FAILURE);
    newNode->data = data;
    newNode->next = NULL;

    if (*head_ref == NULL)
    {
        *head_ref = newNode;
    }
    else
    {
        RecordNode *last = *head_ref;
        while (last->next != NULL)
        {
            last = last->next;
        }
        last->next = newNode;
    }
    return newNode;
}

void delete_from_main_list(RecordNode **head_ref, RecordNode *node_to_delete)
{
    if (*head_ref == NULL || node_to_delete == NULL)
        return;
    if (*head_ref == node_to_delete)
    {
        *head_ref = node_to_delete->next;
        free(node_to_delete);
        return;
    }
    RecordNode *current = *head_ref;
    while (current != NULL && current->next != node_to_delete)
    {
        current = current->next;
    }
    if (current == NULL)
        return;
    current->next = node_to_delete->next;
    free(node_to_delete);
}

void insert_sorted(SortedNode **head_ref, RecordNode *record_node, SortCriteria criteria)
{
    SortedNode *newNode = (SortedNode *)malloc(sizeof(SortedNode));
    if (!newNode)
        exit(EXIT_FAILURE);
    newNode->record_node_ptr = record_node;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (*head_ref == NULL)
    {
        *head_ref = newNode;
        return;
    }

    SortedNode *current = *head_ref;
    while (current != NULL)
    {
        int comparison = 0;
        switch (criteria)
        {
        case BY_ID:
            if (record_node->data.id < current->record_node_ptr->data.id)
                comparison = -1;
            else if (record_node->data.id > current->record_node_ptr->data.id)
                comparison = 1;
            break;
        case BY_NAME:
            comparison = strcmp(record_node->data.name, current->record_node_ptr->data.name);
            break;
        case BY_COST:
            if (record_node->data.cost < current->record_node_ptr->data.cost)
                comparison = -1;
            else if (record_node->data.cost > current->record_node_ptr->data.cost)
                comparison = 1;
            break;
        }

        if (comparison < 0)
        {
            newNode->next = current;
            newNode->prev = current->prev;
            if (current->prev)
            {
                current->prev->next = newNode;
            }
            else
            {
                *head_ref = newNode;
            }
            current->prev = newNode;
            return;
        }
        if (current->next == NULL)
        {
            current->next = newNode;
            newNode->prev = current;
            return;
        }
        current = current->next;
    }
}

void delete_from_sorted_list(SortedNode **head_ref, RecordNode *record_node_to_delete)
{
    if (*head_ref == NULL || record_node_to_delete == NULL)
        return;
    SortedNode *current = *head_ref;
    while (current != NULL)
    {
        if (current->record_node_ptr == record_node_to_delete)
        {
            if (current->prev)
                current->prev->next = current->next;
            else
                *head_ref = current->next;
            if (current->next)
                current->next->prev = current->prev;
            free(current);
            return;
        }
        current = current->next;
    }
}

void load_data_and_build_lists(RecordNode **main_head, SortedNode **id_head, SortedNode **name_head, SortedNode **cost_head, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        printf("File '%s' does not exist. Database is empty.\n", filename);
        return;
    }
    Record temp_record;
    while (fread(&temp_record, sizeof(Record), 1, file) == 1)
    {
        RecordNode *new_main_node = append_to_main_list(main_head, temp_record);
        insert_sorted(id_head, new_main_node, BY_ID);
        insert_sorted(name_head, new_main_node, BY_NAME);
        insert_sorted(cost_head, new_main_node, BY_COST);
    }
    fclose(file);
    printf("Data loaded and sorted lists built.\n");
}

void save_records_to_file(RecordNode *head, const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        perror("Could not open file for writing");
        return;
    }
    RecordNode *current = head;
    while (current != NULL)
    {
        fwrite(&(current->data), sizeof(Record), 1, file);
        current = current->next;
    }
    fclose(file);
    printf("Data saved to file '%s'.\n", filename);
}

void free_all_lists(RecordNode **main_head, SortedNode **id_head, SortedNode **name_head, SortedNode **cost_head)
{
    SortedNode *current_sorted, *next_sorted;

    current_sorted = *id_head;
    while (current_sorted != NULL)
    {
        next_sorted = current_sorted->next;
        free(current_sorted);
        current_sorted = next_sorted;
    }

    current_sorted = *name_head;
    while (current_sorted != NULL)
    {
        next_sorted = current_sorted->next;
        free(current_sorted);
        current_sorted = next_sorted;
    }

    current_sorted = *cost_head;
    while (current_sorted != NULL)
    {
        next_sorted = current_sorted->next;
        free(current_sorted);
        current_sorted = next_sorted;
    }

    RecordNode *current_main, *next_main;
    current_main = *main_head;
    while (current_main != NULL)
    {
        next_main = current_main->next;
        free(current_main);
        current_main = next_main;
    }

    *main_head = NULL;
    *id_head = NULL;
    *name_head = NULL;
    *cost_head = NULL;
}