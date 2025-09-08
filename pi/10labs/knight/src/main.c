#include <stdio.h>

#define N 5

void wypisz(int T[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%3d", T[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int skok(int T[N][N], int w, int k, int i, int *nw, int *nk) {
    int dk[8] = {1, 2, 2, 1, -1, -2, -2, -1};
    int dw[8] = {-2, -1, 1, 2, 2, 1, -1, -2};

    *nw = w + dw[i];
    *nk = k + dk[i];

    if (*nw >= 0 && *nw < N && *nk >= 0 && *nk < N && T[*nw][*nk] == 0) {
        return 1;
    }
    return 0;
}

void ruch(int T[N][N], int w, int k, int c) {
    T[w][k] = c;
    if (c == N * N) {
        wypisz(T);
    } else {
        for (int i = 0; i < 8; i++) {
            int a, b;
            if (skok(T, w, k, i, &a, &b)) {
                ruch(T, a, b, c + 1);
                T[a][b] = 0;
            }
        }
    }
}

int main() {
    int T[N][N] = {0};
    ruch(T, 0, 0, 1);
    return 0;
}
