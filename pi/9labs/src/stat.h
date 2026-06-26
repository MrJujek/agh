#ifndef STATYSTYK_H
#define STATYSTYK_H

#include <stdbool.h>
#include <stdnoreturn.h>

noreturn void pozegnanie();
bool is_number(const char *s, double *value);
double oblicz_sume(const double *arr, int n);
double oblicz_srednia(const double *arr, int n);
double oblicz_mediane(double *arr, int n);
double oblicz_min(const double *arr, int n);
double oblicz_max(const double *arr, int n);

#endif
