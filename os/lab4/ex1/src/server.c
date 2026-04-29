#include "common.h"

int server_queue_id = -1;
int client_queues[MAX_CLIENTS];

void cleanup_queue() {
    if (server_queue_id != -1) {
        printf("\n[SERVER] Removing server queue...\n");
        msgctl(server_queue_id, IPC_RMID, NULL);
    }
}

void sigint_handler(int sig) {
    cleanup_queue();
    exit(0);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        client_queues[i] = -1;
    }

    key_t server_key = get_server_key();
    if (server_key == -1) {
        perror("[SERVER] ftok failed");
        exit(1);
    }

    server_queue_id = msgget(server_key, IPC_CREAT | IPC_EXCL | 0666);
    if (server_queue_id == -1) {
        perror("[SERVER] msgget failed (is server already running?)");
        exit(1);
    }

    printf("[SERVER] Started. Queue ID: %d\n", server_queue_id);

    message_t msg;

    while (1) {
        if (msgrcv(server_queue_id, &msg, sizeof(message_t) - sizeof(long), 0, 0) == -1) {
            perror("[SERVER] msgrcv failed");
            break;
        }

        switch (msg.mtype) {
            case INIT: {
                printf("[SERVER] Received INIT. Client key: %d\n", msg.client_key);
                int client_id = -1;
                for (int i = 0; i < MAX_CLIENTS; ++i) {
                    if (client_queues[i] == -1) {
                        client_id = i;
                        break;
                    }
                }

                if (client_id == -1) {
                    printf("[SERVER] Max clients reached. Rejecting.\n");
                    continue;
                }

                int client_q = msgget(msg.client_key, 0);
                if (client_q == -1) {
                    perror("[SERVER] Failed to open client queue");
                    continue;
                }

                client_queues[client_id] = client_q;
                
                message_t response;
                response.mtype = INIT;
                response.client_id = client_id;
                
                if (msgsnd(client_q, &response, sizeof(message_t) - sizeof(long), 0) == -1) {
                    perror("[SERVER] Failed to send INIT response");
                    client_queues[client_id] = -1;
                } else {
                    printf("[SERVER] Registered client with ID: %d\n", client_id);
                }
                break;
            }
            case MESSAGE: {
                printf("[SERVER] Received MESSAGE from ID %d: %s", msg.client_id, msg.text);
                
                for (int i = 0; i < MAX_CLIENTS; ++i) {
                    if (client_queues[i] != -1 && i != msg.client_id) {
                        if (msgsnd(client_queues[i], &msg, sizeof(message_t) - sizeof(long), 0) == -1) {
                            perror("[SERVER] Failed to forward message");
                        }
                    }
                }
                break;
            }
            case STOP: {
                printf("[SERVER] Received STOP from ID %d\n", msg.client_id);
                if (msg.client_id >= 0 && msg.client_id < MAX_CLIENTS) {
                    client_queues[msg.client_id] = -1;
                }
                break;
            }
            default:
                printf("[SERVER] Received unknown message type\n");
        }
    }

    cleanup_queue();
    return 0;
}
