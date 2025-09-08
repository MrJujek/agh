#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "utils.h"

void browse_list(SortedNode *head, const char *list_name)
{
    if (head == NULL)
    {
        printf("List sorted by '%s' is empty.\n", list_name);
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    SortedNode *current = head;
    int choice = -1;

    while (choice != 0)
    {
        printf("\n--- Browse by: %s ---\n", list_name);
        printf("Current record:\n");
        print_record(&(current->record_node_ptr->data));
        printf("\nBrowse options:\n");
        printf("1. Next record\n");
        printf("2. Previous record\n");
        printf("0. Back to main menu\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1)
        {
            choice = -1;
        }
        while (getchar() != '\n')
            ;

        switch (choice)
        {
        case 1:
            if (current->next)
            {
                current = current->next;
            }
            else
            {
                printf("This is the last record on this list.\n");
            }
            break;
        case 2:
            if (current->prev)
            {
                current = current->prev;
            }
            else
            {
                printf("This is the first record on this list.\n");
            }
            break;
        case 0:
            printf("Returning to main menu.\n");
            break;
        default:
            printf("Unknown option.\n");
            break;
        }
    }
}

int main()
{
    const char *DATABASE_FILENAME = "bin/database.bin";

    RecordNode *main_list_head = NULL;
    SortedNode *sorted_by_id_head = NULL;
    SortedNode *sorted_by_name_head = NULL;
    SortedNode *sorted_by_cost_head = NULL;

    load_data_and_build_lists(&main_list_head, &sorted_by_id_head, &sorted_by_name_head, &sorted_by_cost_head, DATABASE_FILENAME);

    int choice;
    while (1)
    {
        printf("\n--- DATABASE MENU ---\n");
        printf("1. Add new record\n");
        printf("2. Browse sorted data\n");
        printf("3. Save data to file\n");
        printf("4. Delete record (requires Browse)\n");
        printf("5. Edit record (requires Browse)\n");
        printf("0. Save and Exit\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1)
        {
            choice = -1;
        }
        while (getchar() != '\n')
            ;

        switch (choice)
        {
        case 1:
        {
            Record temp_r;
            input_record(&temp_r);
            RecordNode *new_main_node = append_to_main_list(&main_list_head, temp_r);
            insert_sorted(&sorted_by_id_head, new_main_node, BY_ID);
            insert_sorted(&sorted_by_name_head, new_main_node, BY_NAME);
            insert_sorted(&sorted_by_cost_head, new_main_node, BY_COST);
            printf("Record added and indexed in sorted lists.\n");
            break;
        }
        case 2:
        {
            int sort_choice;
            printf("\n--- Select list to browse ---\n");
            printf("1. Sorted by ID\n");
            printf("2. Sorted by Name\n");
            printf("3. Sorted by Cost\n");
            printf("0. Cancel\n");
            printf("Choice: ");
            if (scanf("%d", &sort_choice) != 1)
            {
                sort_choice = -1;
            }
            while (getchar() != '\n')
                ;

            if (sort_choice == 1)
                browse_list(sorted_by_id_head, "ID");
            else if (sort_choice == 2)
                browse_list(sorted_by_name_head, "Name");
            else if (sort_choice == 3)
                browse_list(sorted_by_cost_head, "Cost");
            else if (sort_choice != 0)
                printf("Invalid choice.\n");
            break;
        }
        case 3:
            save_records_to_file(main_list_head, DATABASE_FILENAME);
            break;
        case 4:
        case 5:
            printf("Edit and delete functions are not implemented in this example,\n");
            printf("as they would require more complex user interface logic\n");
            printf("to select a record before the operation. Here is the concept:\n");
            printf("1. The user enters Browse mode (option 2).\n");
            printf("2. In the Browse menu, options 'Edit this record' and 'Delete this record' are added.\n");
            printf("3. After selecting delete, the node is removed from the main list and all sorted lists.\n");
            printf("4. After selecting edit, the data is modified, and the node is then repositioned in the sorted lists.\n");
            break;
        case 0:
            save_records_to_file(main_list_head, DATABASE_FILENAME);
            free_all_lists(&main_list_head, &sorted_by_id_head, &sorted_by_name_head, &sorted_by_cost_head);
            printf("Data saved. Goodbye!\n");
            return 0;
        default:
            printf("Unknown option.\n");
            break;
        }
    }
    return 0;
}