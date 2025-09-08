#include "echo.h"

int work()
{
    struct termios orig_termios, raw;

    int delay_size;
    printf("Podaj wielkość opóźnienia (długość kolejki): ");
    if (scanf("%d", &delay_size) != 1 || delay_size <= 0)
    {
        printf("Opóźnienie musi być liczbą dodatnią.\n");
        return 1;
    }
    while (getchar() != '\n')
        ;

    char *queue = malloc(delay_size * sizeof(char));
    if (queue == NULL)
    {
        return 1;
    }

    int head = 0;
    int count = 0;

    tcgetattr(STDIN_FILENO, &orig_termios);
    raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    printf("Wprowadzaj znaki. Znak '.' kończy program.\r\n");

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != '.')
    {
        putchar(c);

        if (count == delay_size)
        {
            putchar(queue[head]);
            queue[head] = c;
            head = (head + 1) % delay_size;
        }
        else
        {
            queue[count] = c;
            count++;
        }
        fflush(stdout);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);

    free(queue);

    return 0;
}