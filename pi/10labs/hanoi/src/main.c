#include <stdio.h>

#define NUM_DISKS 3

void print_pegs(int peg_A[], int pegAlen, int peg_B[], int pegBlen, int peg_C[], int pegClen) {
    int maxlen = (pegAlen > pegBlen) ? ((pegAlen > pegClen) ? pegAlen : pegClen) : ((pegBlen > pegClen) ? pegBlen : pegClen);

    for (int i = maxlen; i < NUM_DISKS; i++) {
        printf("|\t|\t|\n");
    }

    for (int i = maxlen - 1; i >= 0; i--) {
        char a_val = (i < pegAlen) ? peg_A[i] + '0' : '|';
        char b_val = (i < pegBlen) ? peg_B[i] + '0' : '|';
        char c_val = (i < pegClen) ? peg_C[i] + '0' : '|';

        printf("%c\t%c\t%c\n", a_val, b_val, c_val);
    }
    printf("-----------------\n\n");
}

void move_disk(int from_peg[], int *from_size, int to_peg[], int *to_size) {
    if (*from_size > 0) {
        to_peg[*to_size] = from_peg[*from_size - 1];
        (*to_size)++;
        (*from_size)--;
    }
}

void hanoi(int n, int source[], int *source_size, int target[], int *target_size, int temp[], int *temp_size) {
    if (n > 0) {
        hanoi(n - 1, source, source_size, temp, temp_size, target, target_size);
        move_disk(source, source_size, target, target_size);
        print_pegs(source, *source_size, temp, *temp_size, target, *target_size);
        hanoi(n - 1, temp, temp_size, target, target_size, source, source_size);
    }
}

int main() {
    int peg_A[NUM_DISKS] = {3, 2, 1};
    int peg_B[NUM_DISKS] = {0};
    int peg_C[NUM_DISKS] = {0};
    int pegAlen = NUM_DISKS, pegBlen = 0, pegClen = 0;

    print_pegs(peg_A, pegAlen, peg_B, pegBlen, peg_C, pegClen);
    hanoi(NUM_DISKS, peg_A, &pegAlen, peg_C, &pegClen, peg_B, &pegBlen);

    return 0;
}