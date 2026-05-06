#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

#define STRING_LEN 10

typedef struct {
    int in;
    int out;
    sem_t mutex;
    sem_t empty;
    sem_t full;
    char items[][STRING_LEN + 1];
} SharedData;

SharedData* shared_mem;
int K;
int *child_pids;
int total_children;

void handle_sigint(int sig) {
    (void)sig;
    for (int i = 0; i < total_children; i++) {
        if (child_pids[i] > 0) {
            kill(child_pids[i], SIGTERM);
        }
    }
}

void producer(int id) {
    srand(time(NULL) ^ (getpid() << 16));
    while(1) {
        char str[STRING_LEN + 1];
        for (int i = 0; i < STRING_LEN; i++) {
            str[i] = 'a' + (rand() % 26);
        }
        str[STRING_LEN] = '\0';

        sem_wait(&shared_mem->empty);
        sem_wait(&shared_mem->mutex);

        strcpy(shared_mem->items[shared_mem->in], str);
        printf("[Producent %d] Wygenerowal: %s, zapisal na indeksie %d\n", id, str, shared_mem->in);
        shared_mem->in = (shared_mem->in + 1) % K;

        sem_post(&shared_mem->mutex);
        sem_post(&shared_mem->full);

        usleep(500000 + (rand() % 500000));
    }
}

void consumer(int id) {
    while(1) {
        sem_wait(&shared_mem->full);
        sem_wait(&shared_mem->mutex);

        char str[STRING_LEN + 1];
        strcpy(str, shared_mem->items[shared_mem->out]);
        int idx = shared_mem->out;
        shared_mem->out = (shared_mem->out + 1) % K;

        sem_post(&shared_mem->mutex);
        sem_post(&shared_mem->empty);

        printf("\n[Konsument %d] Pobral z indeksu %d: ", id, idx);
        fflush(stdout);
        for (int i = 0; i < STRING_LEN; i++) {
            putchar(str[i]);
            fflush(stdout);
            usleep(300000);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uzycie: %s <N_producentow> <M_konsumentow> <K_rozmiar_bufora>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    K = atoi(argv[3]);

    if (N <= 0 || M <= 0 || K <= 0) {
        fprintf(stderr, "N, M oraz K musza byc dodatnimi liczbami calkowitymi.\n");
        exit(EXIT_FAILURE);
    }

    total_children = N + M;
    child_pids = malloc(total_children * sizeof(int));
    if (!child_pids) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    size_t size = sizeof(SharedData) + K * (STRING_LEN + 1);
    shared_mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_mem == MAP_FAILED) {
        perror("mmap");
        free(child_pids);
        exit(EXIT_FAILURE);
    }

    shared_mem->in = 0;
    shared_mem->out = 0;

    if (sem_init(&shared_mem->mutex, 1, 1) != 0 ||
        sem_init(&shared_mem->empty, 1, K) != 0 ||
        sem_init(&shared_mem->full, 1, 0) != 0) {
        perror("sem_init");
        munmap(shared_mem, size);
        free(child_pids);
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            producer(i + 1);
            exit(0);
        } else if (pid > 0) {
            child_pids[i] = pid;
        } else {
            perror("fork producenta");
        }
    }

    for (int i = 0; i < M; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            consumer(i + 1);
            exit(0);
        } else if (pid > 0) {
            child_pids[N + i] = pid;
        } else {
            perror("fork konsumenta");
        }
    }

    for (int i = 0; i < total_children; i++) {
        wait(NULL);
    }

    sem_destroy(&shared_mem->mutex);
    sem_destroy(&shared_mem->empty);
    sem_destroy(&shared_mem->full);
    munmap(shared_mem, size);
    free(child_pids);

    printf("\nProces glowny konczy dzialanie. Posprzatano zasoby.\n");
    return 0;
}
