#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void handler(int signum) {
    printf("Signal %d received\n", signum);
}

void sig_default() {
    printf("sig_default() called\n");
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = SIG_DFL;
    sigaction(SIGUSR1, &act, NULL);
}

void sig_mask() {
    printf("sig_mask() called\n");
    sigset_t newmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &newmask, NULL) < 0) {
        perror("Block error");
    }
}

void sig_ignore() {
    printf("sig_ignore() called\n");
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = SIG_IGN;
    sigaction(SIGUSR1, &act, NULL);
}

void sig_handle() {
    printf("sig_handle() called\n");
    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);
}

void sig_unblock() {
    sigset_t unblock_mask;
    sigemptyset(&unblock_mask);
    sigaddset(&unblock_mask, SIGUSR1);
    if (sigprocmask(SIG_UNBLOCK, &unblock_mask, NULL) < 0) {
         perror("Unblock error");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s default|mask|ignore|handle\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "default") == 0) {
        sig_default();
    } else if (strcmp(argv[1], "mask") == 0) {
        sig_mask();
    } else if (strcmp(argv[1], "ignore") == 0) {
        sig_ignore();
    } else if (strcmp(argv[1], "handle") == 0) {
        sig_handle();
    } else {
        printf("Usage: %s default|mask|ignore|handle\n", argv[0]);
        return 1;
    }

    for (int i = 1; i <= 20; ++i) {
        printf("%d\n", i);
        if (i == 5 || i == 15) {
            printf("Sending USR1\n");
            raise(SIGUSR1);
        }
        if (i == 10) {
            sigset_t pending_mask;
            sigpending(&pending_mask);
            if (sigismember(&pending_mask, SIGUSR1)) {
                printf("Unblocking USR1\n");
                sig_unblock();
            }
        }
        sleep(1);
    }

    printf("Loop finished\n");

    return 0;
}