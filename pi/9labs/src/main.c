#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stat.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Użycie: %s <argumenty>\n", argv[0]);
        pozegnanie();
    }

    double liczby[argc - 1];
    int count = 0;

    printf("Pominięto: ");
    for (int i = 1; i < argc; ++i) {
        double val;
        if (is_number(argv[i], &val)) {
            liczby[count++] = val;
        } else {
            printf("%s, ", argv[i]);
        }
    }
    printf("\n");

    if (count == 0) {
        printf("Nie znaleziono żadnych liczb.\n");
        pozegnanie();
    }

    double *do_median = malloc(count * sizeof(double));
    if (!do_median) {
        perror("malloc");
        exit(1);
    }
    memcpy(do_median, liczby, count * sizeof(double));

    printf("Dla liczb:");
    for (int i = 0; i < count; ++i)
        printf(" %g", liczby[i]);
    printf(":\n");

    printf("Suma: %.2f\n", oblicz_sume(liczby, count));
    printf("Średnia: %.2f\n", oblicz_srednia(liczby, count));
    printf("Mediana: %.2f\n", oblicz_mediane(do_median, count));
    printf("W. minimalna: %.2f\n", oblicz_min(liczby, count));
    printf("W. maksymalna: %.2f\n", oblicz_max(liczby, count));

    free(do_median);
    pozegnanie();
}
