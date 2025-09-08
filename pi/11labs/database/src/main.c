#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "utils.h"

int main()
{
    const char *DATABASE_FILENAME = "bin/database.bin";

    RecordNode *main_list_head = NULL;
    RecordNode *last_displayed_record = NULL;

    Record clipboard_data;
    bool clipboard_has_data = false;
    bool insert_after_current_mode = false;

    int choice;
    Record temp_r;

    printf("Próba wczytania danych z pliku %s przy starcie...\n", DATABASE_FILENAME);
    RecordNode *last_loaded_startup = load_records_from_file(&main_list_head, NULL, false, DATABASE_FILENAME);
    if (main_list_head != NULL && last_displayed_record == NULL)
    {
    }

    while (1)
    {
        printf("\n--- MENU GŁÓWNE ---\n");
        printf("Lista: %s | Ostatnio wyświetlony: %s | Tryb wstawiania: %s | Schowek: %s\n",
               main_list_head ? "Niepusta" : "Pusta",
               last_displayed_record ? last_displayed_record->data.name : "Brak",
               insert_after_current_mode ? "PO BIEŻĄCYM" : "NA KOŃCU",
               clipboard_has_data ? "Zawiera dane" : "Pusty");
        printf("1. Wprowadź nowy rekord (użytkownik)\n");
        printf("2. Odczytaj rekordy z pliku (dopisz/wstaw)\n");
        printf("3. Zapisz wszystkie rekordy do pliku\n");
        printf("4. Wyświetl pierwszy rekord\n");
        printf("5. Wyświetl następny rekord\n");
        printf("6. Usuń ostatnio wyświetlony rekord\n");
        printf("7. Przełącz tryb wstawiania (po bieżącym/na końcu)\n");
        printf("8. Kopiuj/Przenieś ostatnio wyświetlony do schowka\n");
        printf("9. Wstaw rekord ze schowka po ostatnio wyświetlonym\n");
        printf("10. Edytuj ostatnio wyświetlony rekord\n");
        printf("0. Wyjdź\n");
        printf("Twój wybór: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Nieprawidłowe wejście. Proszę podać liczbę.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        while (getchar() != '\n')
            ;

        switch (choice)
        {
        case 1:
            printf("Wprowadzanie nowego rekordu:\n");
            input_record(&temp_r);
            if (insert_after_current_mode && last_displayed_record != NULL)
            {
                RecordNode *new_node = insert_record_after_node(last_displayed_record, temp_r);
                if (new_node)
                {
                    last_displayed_record = new_node;
                    printf("Rekord wstawiony po bieżącym.\n");
                }
            }
            else
            {
                append_record_to_list(&main_list_head, temp_r);
                printf("Rekord dodany na końcu listy.\n");
                if (!last_displayed_record && main_list_head)
                {
                    RecordNode *tail = main_list_head;
                    while (tail->next != NULL)
                        tail = tail->next;
                    if (memcmp(&temp_r, &(tail->data), sizeof(Record)) == 0)
                        last_displayed_record = tail;
                }
            }
            break;

        case 2:
            printf("Odczytywanie rekordów z pliku %s...\n", DATABASE_FILENAME);
            RecordNode *last_processed_from_file = load_records_from_file(&main_list_head, last_displayed_record, insert_after_current_mode, DATABASE_FILENAME);
            if (insert_after_current_mode && last_processed_from_file != NULL && last_processed_from_file != last_displayed_record)
            {
                last_displayed_record = last_processed_from_file;
                printf("Ostatnio wyświetlony rekord zaktualizowany na ostatni wczytany z pliku w trybie wstawiania.\n");
            }
            else if (!last_displayed_record && main_list_head)
            {
            }
            break;

        case 3:
            save_records_to_file(main_list_head, DATABASE_FILENAME);
            break;

        case 4:
            if (main_list_head != NULL)
            {
                last_displayed_record = main_list_head;
                printf("\n--- Pierwszy Rekord ---\n");
                print_record(&(last_displayed_record->data));
            }
            else
            {
                printf("Lista jest pusta.\n");
            }
            break;

        case 5:
            if (last_displayed_record != NULL && last_displayed_record->next != NULL)
            {
                last_displayed_record = last_displayed_record->next;
                printf("\n--- Następny Rekord ---\n");
                print_record(&(last_displayed_record->data));
            }
            else if (main_list_head == NULL)
            {
                printf("Lista jest pusta.\n");
            }
            else if (last_displayed_record == NULL && main_list_head != NULL)
            {
                printf("Nie wybrano bieżącego rekordu. Wyświetl pierwszy (opcja 4), aby rozpocząć nawigację.\n");
            }
            else
            {
                printf("To jest ostatni rekord na liście lub lista jest pusta.\n");
            }
            break;

        case 6:
            if (last_displayed_record != NULL)
            {
                RecordNode *current_ldr = last_displayed_record;
                last_displayed_record = delete_selected_node(&main_list_head, current_ldr);
                printf("Rekord usunięty. ");
                if (last_displayed_record)
                {
                    printf("Nowy bieżący rekord (poprzednik lub nowa głowa):\n");
                    print_record(&(last_displayed_record->data));
                }
                else if (main_list_head)
                {
                    printf("Nowy bieżący rekord (nowa głowa):\n");
                    last_displayed_record = main_list_head;
                    print_record(&(last_displayed_record->data));
                }
                else
                {
                    printf("Lista jest teraz pusta lub nie ma poprzednika.\n");
                }
            }
            else
            {
                printf("Nie ma ostatnio wyświetlonego rekordu do usunięcia.\n");
            }
            break;

        case 7:
            insert_after_current_mode = !insert_after_current_mode;
            printf("Tryb wstawiania zmieniony na: %s.\n", insert_after_current_mode ? "WSTAWIAJ PO BIEŻĄCYM" : "DOPISUJ NA KOŃCU");
            break;

        case 8:
            if (last_displayed_record != NULL)
            {
                int sub_choice;
                printf("1. Kopiuj do schowka\n2. Przenieś do schowka\nTwój wybór: ");
                if (scanf("%d", &sub_choice) == 1)
                {
                    while (getchar() != '\n')
                        ;
                    if (sub_choice == 1 || sub_choice == 2)
                    {
                        clipboard_data = last_displayed_record->data;
                        clipboard_has_data = true;
                        printf("Rekord skopiowany do schowka.\n");
                        if (sub_choice == 2)
                        {
                            RecordNode *current_ldr = last_displayed_record;
                            last_displayed_record = delete_selected_node(&main_list_head, current_ldr);
                            printf("Oryginalny rekord usunięty (przeniesiono).\n");
                            if (last_displayed_record)
                            {
                                printf("Nowy bieżący rekord (poprzednik lub nowa głowa):\n");
                                print_record(&(last_displayed_record->data));
                            }
                            else if (main_list_head)
                            {
                                printf("Nowy bieżący rekord (nowa głowa):\n");
                                last_displayed_record = main_list_head;
                                print_record(&(last_displayed_record->data));
                            }
                            else
                            {
                                printf("Lista jest teraz pusta lub nie ma poprzednika.\n");
                            }
                        }
                    }
                    else
                    {
                        printf("Nieprawidłowy wybór.\n");
                    }
                }
                else
                {
                    while (getchar() != '\n')
                        ;
                    printf("Nieprawidłowe wejście.\n");
                }
            }
            else
            {
                printf("Nie ma ostatnio wyświetlonego rekordu.\n");
            }
            break;

        case 9:
            if (clipboard_has_data)
            {
                if (main_list_head == NULL)
                {
                    append_record_to_list(&main_list_head, clipboard_data);
                    last_displayed_record = main_list_head;
                    printf("Rekord ze schowka dodany jako pierwszy element listy.\n");
                }
                else if (last_displayed_record != NULL)
                {
                    RecordNode *new_node = insert_record_after_node(last_displayed_record, clipboard_data);
                    if (new_node)
                    {
                        last_displayed_record = new_node;
                        printf("Rekord ze schowka wstawiony po ostatnio wyświetlonym.\n");
                    }
                }
                else
                {
                    printf("Nie ma ostatnio wyświetlonego rekordu, aby wstawić po nim. Użyj opcji 4 lub 5, aby wybrać rekord, lub dodaj na końcu (niezaimplementowane bezpośrednio dla schowka w tym miejscu).\n");
                }
            }
            else
            {
                printf("Schowek jest pusty.\n");
            }
            break;

        case 10:
            if (last_displayed_record != NULL)
            {
                edit_record_fields_interactive(&(last_displayed_record->data));
                printf("Rekord zaktualizowany.\n");
            }
            else
            {
                printf("Nie ma ostatnio wyświetlonego rekordu do edycji.\n");
            }
            break;

        case 0:
            printf("Zapisywanie danych przed wyjściem...\n");
            save_records_to_file(main_list_head, DATABASE_FILENAME);
            printf("Zwalnianie pamięci...\n");
            free_record_list(&main_list_head);
            printf("Do widzenia!\n");
            return 0;

        default:
            printf("Nieznana opcja. Spróbuj ponownie.\n");
        }
    }
    return 0;
}