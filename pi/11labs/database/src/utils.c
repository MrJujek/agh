#include "utils.h"

const char *offer_str[] = {"Sale", "Rental", "Visit"};

void input_record(Record *r)
{
    printf("Podaj ID (szesnastkowo, np. 1A3F): 0x");
    scanf("%x", &r->id);
    while (getchar() != '\n')
        ;

    printf("Podaj tytuł: ");
    fgets(r->name, NAME_MAX, stdin);
    r->name[strcspn(r->name, "\n")] = '\0';

    printf("Podaj rok: ");
    scanf("%d", &r->year);
    while (getchar() != '\n')
        ;

    printf("Typ oferty (0 - Sprzedaż, 1 - Wynajem, 2 - Wizyta): ");
    int type;
    scanf("%d", &type);
    while (getchar() != '\n')
        ;
    if (type >= SALE && type <= VISIT)
    {
        r->offer = (OfferType)type;
    }
    else
    {
        printf("Nieprawidłowy typ oferty, ustawiono domyślnie na Sprzedaż.\n");
        r->offer = SALE;
    }

    printf("Podaj koszt (PLN): ");
    scanf("%f", &r->cost);
    while (getchar() != '\n')
        ;

    printf("Podaj lokalizację: ");
    fgets(r->location, LOCATION_MAX, stdin);
    r->location[strcspn(r->location, "\n")] = '\0';
}

void print_record(const Record *r)
{
    if (!r)
    {
        printf("Brak rekordu do wyświetlenia.\n");
        return;
    }
    printf("ID: 0x%X\nNazwa: %s\nRok: %d\nOferta: %s\nKoszt: %.2f PLN\nLokalizacja: %s\n\n",
           r->id, r->name, r->year, offer_str[r->offer], r->cost, r->location);
}

void edit_record_fields_interactive(Record *r)
{
    if (!r)
    {
        printf("Brak rekordu do edycji.\n");
        return;
    }
    char buffer[256];
    int choice;
    float temp_float;
    int temp_int;

    printf("\n--- Edycja Rekordu ---\n");
    print_record(r);

    printf("Czy chcesz zmienić ID (0x%X)? (1-Tak, 0-Nie): ", r->id);
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%d", &choice) == 1 && choice == 1)
    {
        printf("Nowe ID (szesnastkowo): 0x");
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%x", &r->id);
    }

    printf("Czy chcesz zmienić nazwę (%s)? (1-Tak, 0-Nie): ", r->name);
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%d", &choice) == 1 && choice == 1)
    {
        printf("Nowa nazwa: ");
        fgets(r->name, NAME_MAX, stdin);
        r->name[strcspn(r->name, "\n")] = '\0';
    }

    printf("Czy chcesz zmienić rok (%d)? (1-Tak, 0-Nie): ", r->year);
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%d", &choice) == 1 && choice == 1)
    {
        printf("Nowy rok: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &temp_int) == 1)
            r->year = temp_int;
    }

    printf("Czy chcesz zmienić typ oferty (%s)? (1-Tak, 0-Nie): ", offer_str[r->offer]);
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%d", &choice) == 1 && choice == 1)
    {
        printf("Nowy typ oferty (0-Sprzedaż, 1-Wynajem, 2-Wizyta): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &temp_int) == 1 && temp_int >= SALE && temp_int <= VISIT)
        {
            r->offer = (OfferType)temp_int;
        }
        else
        {
            printf("Nieprawidłowy typ, pozostawiono stary.\n");
        }
    }

    printf("Czy chcesz zmienić koszt (%.2f PLN)? (1-Tak, 0-Nie): ", r->cost);
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%d", &choice) == 1 && choice == 1)
    {
        printf("Nowy koszt (PLN): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%f", &temp_float) == 1)
            r->cost = temp_float;
    }

    printf("Czy chcesz zmienić lokalizację (%s)? (1-Tak, 0-Nie): ", r->location);
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%d", &choice) == 1 && choice == 1)
    {
        printf("Nowa lokalizacja: ");
        fgets(r->location, LOCATION_MAX, stdin);
        r->location[strcspn(r->location, "\n")] = '\0';
    }
    printf("Edycja zakończona.\n");
}

RecordNode *create_record_node(Record data)
{
    RecordNode *newNode = (RecordNode *)malloc(sizeof(RecordNode));
    if (!newNode)
    {
        perror("Błąd alokacji pamięci dla nowego węzła");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void append_record_to_list(RecordNode **head_ref, Record data)
{
    RecordNode *newNode = create_record_node(data);
    if (*head_ref == NULL)
    {
        *head_ref = newNode;
        return;
    }
    RecordNode *last = *head_ref;
    while (last->next != NULL)
    {
        last = last->next;
    }
    last->next = newNode;
}

RecordNode *insert_record_after_node(RecordNode *prev_node, Record data)
{
    if (prev_node == NULL)
    {
        printf("Błąd: Próba wstawienia po węźle NULL.\n");
        return NULL;
    }
    RecordNode *newNode = create_record_node(data);
    newNode->next = prev_node->next;
    prev_node->next = newNode;
    return newNode;
}

RecordNode *load_records_from_file(RecordNode **head_ref, RecordNode *insert_after_this_node, bool insert_mode, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        printf("Plik %s nie został znaleziony lub nie można go otworzyć. Nie wczytano rekordów.\n", filename);
        return insert_after_this_node;
    }

    Record temp_record;
    RecordNode *current_insertion_point = (insert_mode && insert_after_this_node) ? insert_after_this_node : NULL;
    RecordNode *last_node_processed_from_file = current_insertion_point;

    printf("Wczytywanie rekordów z pliku %s...\n", filename);
    while (fread(&temp_record, sizeof(Record), 1, file) == 1)
    {
        if (insert_mode && current_insertion_point != NULL)
        {
            RecordNode *new_node = create_record_node(temp_record);
            new_node->next = current_insertion_point->next;
            current_insertion_point->next = new_node;
            current_insertion_point = new_node;
            last_node_processed_from_file = new_node;
        }
        else
        {
            append_record_to_list(head_ref, temp_record);
            if (*head_ref != NULL && last_node_processed_from_file == NULL && insert_mode)
            {
                RecordNode *tail = *head_ref;
                while (tail->next != NULL)
                    tail = tail->next;
                last_node_processed_from_file = tail;
            }
            else if (!insert_mode)
            {
                RecordNode *tail = *head_ref;
                if (tail)
                { // Check if head is not NULL
                    while (tail->next != NULL)
                        tail = tail->next;
                    last_node_processed_from_file = tail;
                }
            }
            if (insert_mode && current_insertion_point == NULL && *head_ref != NULL)
            {
                RecordNode *tail = *head_ref;
                while (tail->next != NULL)
                    tail = tail->next;
                current_insertion_point = tail;
                last_node_processed_from_file = current_insertion_point;
            }
        }
    }
    fclose(file);
    printf("Zakończono wczytywanie rekordów.\n");
    return last_node_processed_from_file;
}

void save_records_to_file(RecordNode *head, const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        perror("Nie można otworzyć pliku do zapisu");
        printf("Upewnij się, że katalog 'bin' istnieje.\n");
        return;
    }

    RecordNode *current = head;
    while (current != NULL)
    {
        if (fwrite(&(current->data), sizeof(Record), 1, file) != 1)
        {
            perror("Błąd podczas zapisu rekordu do pliku");
            fclose(file);
            return;
        }
        current = current->next;
    }
    printf("Wszystkie rekordy zostały zapisane do pliku %s.\n", filename);
    fclose(file);
}

RecordNode *delete_selected_node(RecordNode **head_ref, RecordNode *node_to_delete)
{
    if (*head_ref == NULL || node_to_delete == NULL)
    {
        return NULL;
    }

    RecordNode *temp_prev = NULL;

    if (*head_ref == node_to_delete)
    {
        *head_ref = node_to_delete->next;
        free(node_to_delete);
        return *head_ref;
    }

    RecordNode *current = *head_ref;
    while (current != NULL && current->next != node_to_delete)
    {
        current = current->next;
    }

    if (current == NULL || current->next == NULL)
    {
        return node_to_delete;
    }

    temp_prev = current;
    current->next = node_to_delete->next;
    free(node_to_delete);

    return temp_prev;
}

void free_record_list(RecordNode **head_ref)
{
    RecordNode *current = *head_ref;
    RecordNode *next_node;
    while (current != NULL)
    {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    *head_ref = NULL;
}