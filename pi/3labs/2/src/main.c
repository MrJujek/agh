#include <stdio.h>
#include "dzialania.h"

int main(void)
{
    akcja(td_zerowanie, 2, 3, 5.0);
    printf("\n%f\n", akcja(td_odczyt, 2, 3, 5.0));
    return 0;
}