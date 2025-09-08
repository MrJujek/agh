#include <stdio.h>
#include "table_actions.h"

// #define USE_POINTERS

void set_zeroes(float *array, int size)
{
#ifdef USE_POINTERS
    for (float *ptr = array; ptr < array + size; ptr++)
    {
        *ptr = 0.0f;
    }
#else
    for (int i = 0; i < size; i++)
    {
        array[i] = 0.0f;
    }
#endif
}

void print_range(const float *array, int start, int end)
{
#ifdef USE_POINTERS
    for (const float *ptr = array + start; ptr <= array + end; ptr++)
    {
        printf("%.2f ", *ptr);
    }
#else
    for (int i = start; i <= end; i++)
    {
        printf("%.2f ", array[i]);
    }
#endif
    printf("\n");
}

void assign_value(float *array, int index, float value)
{
#ifdef USE_POINTERS
    *(array + index) = value;
#else
    array[index] = value;
#endif
}

float sum_range(const float *array, int start, int end)
{
    float sum = 0.0f;
#ifdef USE_POINTERS
    for (const float *ptr = array + start; ptr <= array + end; ptr++)
    {
        sum += *ptr;
    }
#else
    for (int i = start; i <= end; i++)
    {
        sum += array[i];
    }
#endif
    return sum;
}

int count_different(const float *array, int size, float value)
{
    int count = 0;
#ifdef USE_POINTERS
    for (const float *ptr = array; ptr < array + size; ptr++)
    {
        if (*ptr != value)
        {
            count++;
        }
    }
#else
    for (int i = 0; i < size; i++)
    {
        if (array[i] != value)
        {
            count++;
        }
    }
#endif
    return count;
}

void swap_values(float *array, int index1, int index2)
{
#ifdef USE_POINTERS
    float temp = *(array + index1);
    *(array + index1) = *(array + index2);
    *(array + index2) = temp;
#else
    float temp = array[index1];
    array[index1] = array[index2];
    array[index2] = temp;
#endif
}