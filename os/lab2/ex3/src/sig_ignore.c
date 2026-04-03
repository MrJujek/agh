#include <stdio.h>
#include <signal.h>
#include "handlers.h"

void sig_ignore() {
    printf("sig_ignore() called\n");
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = SIG_IGN;
    sigaction(SIGUSR1, &act, NULL);
}
