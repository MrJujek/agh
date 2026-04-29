#include "common.h"

int client_queue_id = -1;
int server_queue_id = -1;
int client_id = -1;
pid_t child_pid = -1;

void cleanup() {
    if (client_queue_id != -1) {
        msgctl(client_queue_id, IPC_RMID, NULL);
    }
    if (child_pid > 0) {
        kill(child_pid, SIGINT);
    }
}

void sigint_handler(int sig) {
    if (child_pid == 0) {
        exit(0);
    }

    if (client_id != -1 && server_queue_id != -1) {
        message_t msg;
        msg.mtype = STOP;
        msg.client_id = client_id;
        msgsnd(server_queue_id, &msg, sizeof(message_t) - sizeof(long), 0);
        server_queue_id = -1;
    }
    cleanup();
    exit(0);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    key_t client_key = ftok(getenv("HOME"), getpid());
    if (client_key == -1) {
        perror("[CLIENT] ftok failed");
        exit(1);
    }

    client_queue_id = msgget(client_key, IPC_CREAT | IPC_EXCL | 0666);
    if (client_queue_id == -1) {
        perror("[CLIENT] msgget failed");
        exit(1);
    }

    printf("[CLIENT] Started. Queue ID: %d\n", client_queue_id);

    key_t server_key = get_server_key();
    server_queue_id = msgget(server_key, 0);
    if (server_queue_id == -1) {
        perror("[CLIENT] Failed to connect to server queue. Is server running?");
        cleanup();
        exit(1);
    }

    message_t msg;
    msg.mtype = INIT;
    msg.client_key = client_key;
    
    if (msgsnd(server_queue_id, &msg, sizeof(message_t) - sizeof(long), 0) == -1) {
        perror("[CLIENT] Failed to send INIT to server");
        cleanup();
        exit(1);
    }

    if (msgrcv(client_queue_id, &msg, sizeof(message_t) - sizeof(long), INIT, 0) == -1) {
        perror("[CLIENT] Failed to receive INIT response");
        cleanup();
        exit(1);
    }

    client_id = msg.client_id;
    printf("[CLIENT] Successfully registered with ID: %d\n", client_id);

    child_pid = fork();
    if (child_pid == -1) {
        perror("[CLIENT] fork failed");
        cleanup();
        exit(1);
    }

    if (child_pid == 0) {
        while (1) {
            message_t rcv_msg;
            if (msgrcv(client_queue_id, &rcv_msg, sizeof(message_t) - sizeof(long), 0, 0) == -1) {
                break;
            }
            if (rcv_msg.mtype == MESSAGE) {
                printf("\n[From Client %d]: %s", rcv_msg.client_id, rcv_msg.text);
                printf("Enter message: ");
                fflush(stdout);
            }
        }
        exit(0);
    } else {
        char buffer[MAX_MSG_LEN];
        while (1) {
            printf("Enter message: ");
            if (fgets(buffer, MAX_MSG_LEN, stdin) == NULL) {
                break;
            }

            if (strncmp(buffer, "STOP\n", 5) == 0) {
                break;
            }

            message_t snd_msg;
            snd_msg.mtype = MESSAGE;
            snd_msg.client_id = client_id;
            strncpy(snd_msg.text, buffer, MAX_MSG_LEN);
            
            if (msgsnd(server_queue_id, &snd_msg, sizeof(message_t) - sizeof(long), 0) == -1) {
                perror("[CLIENT] Failed to send message to server");
                break;
            }
        }
        
        sigint_handler(SIGINT);
    }

    return 0;
}
