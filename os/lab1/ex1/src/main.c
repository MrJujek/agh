#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define M 5

int global = 10;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s N\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    pid_t pid;

    for (int i = 0; i < N; i++)
    {
        pid = fork();

        if (pid < 0)
        {
            perror("fork");
            exit(1);
        }

        if (pid == 0)
        {
            global++;

            for (int j = 0; j < M; j++)
            {
                printf("Child\t(PID: %d)\n", getpid());
                fflush(stdout);
                sleep(1);
            }

            exit(0);
        }
    }

    for (int i = 0; i < N; i++)
    {
        wait(NULL);
    }

    printf("Parent\t(%d) global=%d\n", getpid(), global);

    return 0;
}