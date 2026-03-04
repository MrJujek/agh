#include "definitions.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s M\n", argv[0]);
        return 1;
    }

    int M = atoi(argv[1]);

    FILE *file = fopen(FILENAME, "a");
    if (file == NULL)
    {
        perror("Error during oppening file");
        return 1;
    }

    int fd = fileno(file);

    if (flock(fd, LOCK_EX) == -1)
    {
        perror("Error during locking file");
        fclose(file);
        return 1;
    }

    for (int i = 0; i < M; i++)
    {
        fprintf(file, "Child\t(PID: %d)\n", getpid());

        fflush(file);

        sleep(1);
    }

    flock(fd, LOCK_UN);

    fclose(file);

    return 0;
}