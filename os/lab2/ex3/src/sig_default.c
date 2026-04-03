#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <signal.h>
#include "handlers.h"

void sig_default() {
    printf("sig_default() called\n");
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = SIG_DFL;
    sigaction(SIGUSR1, &act, NULL);
}
