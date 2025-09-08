#include <stdio.h>
#include "tablicaliczb.h"

int main(void)
{
    print_table(2, 5);
    set_in_table(1.23, 2);
    set_in_table(2.34, 5);
    print_table(2, 5);
    printf("%f\n", get_sum(2, 5));
    printf("%d\n", get_diffrent(1.23));
    swap(2, 5);
    print_table(2, 5);
    zero_table();
    print_table(2, 5);
    return 0;
}