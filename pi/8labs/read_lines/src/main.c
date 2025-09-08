#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

int read_char_lines(char ***lines) {
    int n, i;
    printf("Podaj liczbę wierszy: ");
    scanf("%d", &n);
    getchar();

    *lines = (char **)malloc(n * sizeof(char *));
    if (*lines == NULL) {
        fprintf(stderr, "Błąd alokacji pamięci.\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < n; i++) {
        printf("Wpisz wiersz %d: ", i + 1);
        char buffer[MAX_LINE_LENGTH];
        if (fgets(buffer, MAX_LINE_LENGTH, stdin) == NULL) {
            fprintf(stderr, "Błąd odczytu linii.\n");
            exit(EXIT_FAILURE);
        }

        size_t len = strlen(buffer);
        if (buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        (*lines)[i] = (char *)malloc((len + 1) * sizeof(char));
        if ((*lines)[i] == NULL) {
            fprintf(stderr, "Błąd alokacji pamięci.\n");
            exit(EXIT_FAILURE);
        }

        strcpy((*lines)[i], buffer);
    }

    return n;
}

void write_char_line(char **lines, int index, int count) {
    if (index < 1 || index > count) {
        printf("Nieprawidłowy numer wiersza.\n");
        return;
    }
    printf("%s\n", lines[index - 1]);
}

void delete_lines(char **lines, int count) {
    for (int i = 0; i < count; i++) {
        free(lines[i]);
    }
    free(lines);
}

int main() {
    char **lines;
    int count = read_char_lines(&lines);

    int index;
    printf("Podaj numer wiersza do wyświetlenia: ");
    scanf("%d", &index);

    write_char_line(lines, index, count);
    delete_lines(lines, count);

    return 0;
}
