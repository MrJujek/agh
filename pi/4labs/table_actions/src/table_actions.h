#ifndef TABELE_AKCJE_H
#define TABELE_AKCJE_H

void set_zeroes(float *array, int size);

void print_range(const float *array, int start, int end);

void assign_value(float *array, int index, float value);

float sum_range(const float *array, int start, int end);

int count_different(const float *array, int size, float value);

void swap_values(float *array, int index1, int index2);

#endif
