#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stat.h"

noreturn void pozegnanie() {
    printf("\nDziękuję za wybranie programu Statystyk.\n");
    exit(0);
}

bool is_number(const char *s, double *value) {
    char *end;
    *value = strtod(s, &end);
    return (end != s && *end == '\0');
}

int compare_doubles(const void *a, const void *b) {
    double da = *(const double*)a;
    double db = *(const double*)b;
    return (da > db) - (da < db);
}

double oblicz_sume(const double *arr, int n) {
    double suma = 0;
    for (int i = 0; i < n; ++i)
        suma += arr[i];
    return suma;
}

double oblicz_srednia(const double *arr, int n) {
    if (n == 0) return 0;
    return oblicz_sume(arr, n) / n;
}

double oblicz_mediane(double *arr, int n) {
    if (n == 0) return 0;
    qsort(arr, n, sizeof(double), compare_doubles);
    if (n % 2 == 1)
        return arr[n / 2];
    else
        return (arr[n/2 - 1] + arr[n/2]) / 2.0;
}

double oblicz_min(const double *arr, int n) {
    if (n == 0) return 0;
    double min = arr[0];
    for (int i = 1; i < n; ++i)
        if (arr[i] < min) min = arr[i];
    return min;
}

double oblicz_max(const double *arr, int n) {
    if (n == 0) return 0;
    double max = arr[0];
    for (int i = 1; i < n; ++i)
        if (arr[i] > max) max = arr[i];
    return max;
}
