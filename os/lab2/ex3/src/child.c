#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#ifdef DYNAMIC
#include <dlfcn.h>
#else
#include "handlers.h"
#endif

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

#ifdef DYNAMIC
    void *handle = dlopen("./bin/libhandlers.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Cannot load dynamic library: %s\n", dlerror());
        return;
    }

    void (*func)(void) = NULL;
    if (reaction == 0) {
        *(void **)(&func) = dlsym(handle, "sig_default");
    } else if (reaction == 1) {
        *(void **)(&func) = dlsym(handle, "sig_mask");
    } else if (reaction == 2) {
        *(void **)(&func) = dlsym(handle, "sig_ignore");
    } else if (reaction == 3) {
        *(void **)(&func) = dlsym(handle, "sig_handle");
    } else {
        printf("Unknown reaction\n");
        dlclose(handle);
        return;
    }

    if (func) {
        func();
    } else {
        fprintf(stderr, "Cannot find symbol: %s\n", dlerror());
    }
    
    // dlclose(handle); 
#else
    if (reaction == 0) {
        sig_default();
    } else if (reaction == 1) {
        sig_mask();
    } else if (reaction == 2) {
        sig_ignore();
    } else if (reaction == 3) {
        sig_handle();
    } else {
        printf("Unknown reaction\n");
    }
#endif
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
