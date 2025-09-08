#include <stdio.h>
#include "table_actions.h"

int main()
{
    float array[10] = {1.5f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f, 10.0f};

    print_range(array, 0, 9);

    set_zeroes(array, 10);
    print_range(array, 0, 9);

    assign_value(array, 5, 42.0f);
    print_range(array, 0, 9);

    assign_value(array, 2, 25.0f);
    print_range(array, 0, 9);

    float sum = sum_range(array, 2, 5);
    printf("%.2f\n", sum);

    int count = count_different(array, 10, 0.0f);
    printf("%d\n", count);

    swap_values(array, 2, 5);
    print_range(array, 0, 9);

    return 0;
}
