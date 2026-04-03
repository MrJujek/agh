#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <ucontext.h>

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

void usr2_handler(int signum, siginfo_t *info, void *context) {
    (void)signum;
    (void)context;
    int reaction = info->si_value.sival_int;

    if (reaction == 0) {
        sig_default();
    } else if (reaction == 1) {
        // printf("sig_mask() called via context\n");
        // ucontext_t *uc = (ucontext_t *)context;
        // sigaddset(&(uc->uc_sigmask), SIGUSR1);
        sig_mask();
    } else if (reaction == 2) {
        sig_ignore();
    } else if (reaction == 3) {
        sig_handle();
    } else {
        printf("Unknown reaction\n");
    }
}

int main()
{
    struct sigaction act;
    act.sa_sigaction = usr2_handler;
    act.sa_flags = SA_SIGINFO;
    sigemptyset(&act.sa_mask);
    sigaction(SIGUSR2, &act, NULL);

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