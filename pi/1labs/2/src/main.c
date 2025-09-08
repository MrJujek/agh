#include <stdio.h>
#include "utils.h"
#include "math_utils.h"
#include "primes.h"

int main(void)
{
    print_message();
    printf("2 + 3 = %d\n", add(2, 3));
    printf("10 / 5 = %d\n", divide(10, 5));
    printf("2 ^ 5 = %d\n", power(2, 5));
    printf("Is 5 even? %d\n", isEven(5));
    primes();
    return 0;
}
