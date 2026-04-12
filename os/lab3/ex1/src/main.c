#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

double f(double x) {
    return 4.0 / (x * x + 1.0);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <rectangle_width> <n>\n", argv[0]);
        return 1;
    }

    double w = atof(argv[1]);
    int n = atoi(argv[2]);

    if (w <= 0.0 || w > 1.0 || n <= 0) {
        fprintf(stderr, "Invalid arguments. Width must be in (0, 1] and n > 0.\n");
        return 1;
    }

    long long N = (long long)(1.0 / w);

    for (int k = 1; k <= n; ++k) {
        int (*pipes)[2] = malloc((size_t)k * sizeof(*pipes));
        if (!pipes) {
            perror("malloc");
            return 1;
        }

        for (int i = 0; i < k; ++i) {
            if (pipe(pipes[i]) == -1) {
                perror("pipe");
                free(pipes);
                return 1;
            }
        }

        struct timespec start_time, end_time;
        if (clock_gettime(CLOCK_MONOTONIC, &start_time) == -1) {
            perror("clock_gettime");
            free(pipes);
            return 1;
        }

        for (int i = 0; i < k; ++i) {
            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
                return 1;
            } else if (pid == 0) {
                for (int j = 0; j < k; ++j) {
                    close(pipes[j][0]);
                    if (j != i) {
                        close(pipes[j][1]);
                    }
                }

                long long start_rect = i * N / k;
                long long end_rect = (i + 1) * N / k;
                if (i == k - 1) {
                    end_rect = N;
                }

                double local_sum = 0.0;
                for (long long j = start_rect; j < end_rect; ++j) {
                    double x = j * w + w / 2.0;
                    local_sum += f(x) * w;
                }

                if (write(pipes[i][1], &local_sum, sizeof(local_sum)) != sizeof(local_sum)) {
                    perror("write");
                }
                close(pipes[i][1]);
                free(pipes);
                exit(0);
            }
        }

        for (int i = 0; i < k; ++i) {
            close(pipes[i][1]);
        }

        double total_sum = 0.0;
        for (int i = 0; i < k; ++i) {
            double local_sum;
            if (read(pipes[i][0], &local_sum, sizeof(local_sum)) == sizeof(local_sum)) {
                total_sum += local_sum;
            } else {
                perror("read");
            }
            close(pipes[i][0]);
        }

        for (int i = 0; i < k; ++i) {
            wait(NULL);
        }

        if (clock_gettime(CLOCK_MONOTONIC, &end_time) == -1) {
            perror("clock_gettime");
        }
        
        double elapsed = (double)(end_time.tv_sec - start_time.tv_sec) + 
                         (double)(end_time.tv_nsec - start_time.tv_nsec) / 1e9;

        printf("k = %d, Result = %.10f, Time = %.6f s\n", k, total_sum, elapsed);
        
        free(pipes);
    }

    return 0;
}