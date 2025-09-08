#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int main()
{
    FILE *file = fopen("bin/database.bin", "r+b");
    if (!file)
    {
        file = fopen("bin/database.bin", "w+b");
    }

    int choice;
    Record r;
    int index = 0;

    while (1)
    {
        printf("\n--- MENU ---\n");
        printf("1. Add record\n2. Show current record\n3. Previous\n4. Next\n5. Search by name\n6. Search by budget\n7. Change cost\n8. Average cost\n0. Exit\nYour choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            input_record(&r);
            save_record(file, &r);
            break;
        case 2:
            if (read_record(file, index, &r))
            {
                print_record(&r);
            }
            else
            {
                printf("No record found\n");
            }
            break;
        case 3:
            if (index > 0)
            {
                index--;
            }
            else
            {
                printf("This is the first record.\n");
            }
            break;
        case 4:
            index++;
            if (!read_record(file, index, &r))
            {
                printf("This is the last record.\n");
                index--;
            }
            else
            {
                print_record(&r);
            }
            break;
        case 5:
        {
            char frag[NAME_MAX];
            printf("Enter part of the name: ");
            fgets(frag, NAME_MAX, stdin);
            frag[strcspn(frag, "\n")] = '\0';
            search_by_name(file, frag);
        }
        break;
        case 6:
        {
            float budget;
            printf("Enter maximum budget: ");
            scanf("%f", &budget);
            search_by_budget(file, budget);
        }
        break;
        case 7:
        {
            unsigned int id;
            float cost;
            printf("Enter ID (hex): 0x");
            scanf("%x", &id);
            printf("New cost: ");
            scanf("%f", &cost);
            if (!change_cost(file, id, cost))
            {
                printf("Record not found.\n");
            }
        }
        break;
        case 8:
            average_cost(file);
            break;
        case 0:
            fclose(file);
            return 0;
        default:
            printf("Unknown option.\n");
        }
    }
}
