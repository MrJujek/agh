#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s default|mask|ignore|handle\n", argv[0]);
        return 1;
    }

    int reaction = -1;
    if (strcmp(argv[1], "default") == 0) {
        reaction = 0;
    } else if (strcmp(argv[1], "mask") == 0) {
        reaction = 1;
    } else if (strcmp(argv[1], "ignore") == 0) {
        reaction = 2;
    } else if (strcmp(argv[1], "handle") == 0) {
        reaction = 3;
    } else {
        printf("Usage: %s default|mask|ignore|handle\n", argv[0]);
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        if (execl("./bin/child", "child", NULL) == -1) {
            if (execl("./child", "child", NULL) == -1) {
                perror("execl");
                exit(1);
            }
        }
    } else {
        usleep(100000);
        
        union sigval value;
        value.sival_int = reaction;
        printf("Parent: sending USR2 to child %d with reaction id %d\n", pid, reaction);
        if (sigqueue(pid, SIGUSR2, value) != 0) {
            perror("sigqueue");
        }
        
        wait(NULL);
    }

    return 0;
}
