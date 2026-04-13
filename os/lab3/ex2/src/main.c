#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    char *req_fifo = "/tmp/calc_req_fifo";
    char *res_fifo = "/tmp/calc_res_fifo";

    mkfifo(req_fifo, 0666);
    mkfifo(res_fifo, 0666);

    double a, b;
    double w = 0.000000001;
    
    if (argc >= 2) {
        w = atof(argv[1]);
    } else {
        printf("Precision (w) not provided as argument. Using default %.10f\n", w);
    }

    printf("Enter interval [a, b]: ");
    if (scanf("%lf %lf", &a, &b) != 2) {
        fprintf(stderr, "Invalid interval\n");
        return 1;
    }

    int fd_req = open(req_fifo, O_WRONLY);
    if (fd_req == -1) {
        perror("open req_fifo");
        return 1;
    }

    double params[3] = {a, b, w};
    if (write(fd_req, params, sizeof(params)) != sizeof(params)) {
        perror("write");
        close(fd_req);
        return 1;
    }
    close(fd_req);

    int fd_res = open(res_fifo, O_RDONLY);
    if (fd_res == -1) {
        perror("open res_fifo");
        return 1;
    }

    double result;
    if (read(fd_res, &result, sizeof(result)) > 0) {
        printf("Result: %.10f\n", result);
    } else {
        perror("read result");
    }

    close(fd_res);

    unlink(req_fifo);
    unlink(res_fifo);

    return 0;
}
