#include <stdio.h>
#include <assert.h>
#include "../src/math_utils.h"

void test_add() {
    assert(add(2,3) == 5);
    assert(add(-1, 1) == 0);
    printf("done");
}

int main() {
    test_add();
    printf("done done");
    return 0;
}