#include "definitions.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s N M\n", argv[0]);
        return 1;
    }

    unlink(FILENAME);

    int N = atoi(argv[1]);

    for (int i = 0; i < N; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("Error (fork)");
            exit(1);
        }
        else if (pid == 0)
        {
            execl("./child", "child", argv[2], NULL);

            perror("Error (execl)");
            exit(1);
        }
    }

    for (int i = 0; i < N; i++)
    {
        wait(NULL);
    }

    printf("Parent\t(PID: %d)\n", getpid());

    return 0;
}