#include <stdio.h>
#include <signal.h>
#include "handlers.h"

void sig_mask() {
    printf("sig_mask() called\n");
    sigset_t newmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &newmask, NULL) < 0) {
        perror("Block error");
    }
}
