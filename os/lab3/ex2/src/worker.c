#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

double f(double x) {
    return 4.0 / (x * x + 1.0);
}

int main(void) {
    char *req_fifo = "/tmp/calc_req_fifo";
    char *res_fifo = "/tmp/calc_res_fifo";

    mkfifo(req_fifo, 0666);
    mkfifo(res_fifo, 0666);

    printf("Worker is running and waiting for requests on %s...\n", req_fifo);

    int fd_req = open(req_fifo, O_RDONLY);
    if (fd_req == -1) {
        perror("open req_fifo");
        return 1;
    }

    double params[3];
    ssize_t bytes_read = read(fd_req, params, sizeof(params));
    close(fd_req);

    if (bytes_read == sizeof(params)) {
        double a = params[0];
        double b = params[1];
        double w = params[2];

        if (w <= 0.0) {
            w = 0.0001;
        }

        if (a > b) {
            double temp = a;
            a = b;
            b = temp;
        }

        long long N = (long long)((b - a) / w);
        double sum = 0.0;
        
        for (long long j = 0; j < N; ++j) {
            double mid = a + j * w + w / 2.0;
            sum += f(mid) * w;
        }
        
        double remaining = (b - a) - (N * w);
        if (remaining > 1e-15) {
            double mid = a + N * w + remaining / 2.0;
            sum += f(mid) * remaining;
        }

        int fd_res = open(res_fifo, O_WRONLY);
        if (fd_res != -1) {
            write(fd_res, &sum, sizeof(sum));
            close(fd_res);
        } else {
            perror("open res_fifo");
            return 1;
        }
        printf("Worker finished calculating. Result %.10f sent.\n", sum);
    } else {
        fprintf(stderr, "Failed to read properly formatted interval\n");
        return 1;
    }

    return 0;
}
