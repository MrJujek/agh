#include <stdio.h>
#include <signal.h>
#include "handlers.h"

static void handler(int signum) {
    printf("Signal %d received\n", signum);
}

void sig_handle() {
    printf("sig_handle() called\n");
    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);
}
