#ifndef TABLICALICZB_H
#define TABLICALICZB_H

extern float numbers[100];

void zero_table();

void print_table(int start, int end);

void set_in_table(float number, int index);

float get_sum(int start, int end);

int get_diffrent(float number);

void swap(int first, int second);

#endif