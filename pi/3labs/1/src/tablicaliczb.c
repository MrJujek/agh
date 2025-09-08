#include <stdio.h>

float numbers[100];

void zero_table()
{
    for (int i = 0; i < 100; i++)
    {
        numbers[i] = 0;
    }
}

void print_table(int start, int end)
{
    for (; start <= end; start++)
    {
        printf("%f ", numbers[start]);
    }
    printf("\n");
}

void set_in_table(float number, int index)
{
    numbers[index] = number;
}

float get_sum(int start, int end)
{
    float sum = 0;
    for (; start <= end; start++)
    {
        sum += numbers[start];
    }
    return sum;
}

int get_diffrent(float number)
{
    int count = 0;
    for (int i = 0; i < 100; i++)
    {
        if (numbers[i] != number)
        {
            count++;
        }
    }

    return count;
}

void swap(int first, int second)
{
    float temp = numbers[first];
    numbers[first] = numbers[second];
    numbers[second] = temp;
}