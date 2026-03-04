#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s M\n", argv[0]);
        return 1;
    }

    int M = atoi(argv[1]);

    for (int i = 0; i < M; i++)
    {
        printf("Child\t(PID: %d)\n", getpid());
        sleep(1);
    }

    return 0;
}