#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <pwd.h>
#include <time.h>
#include <errno.h>

#define MAX_MSG_LEN 1024
#define MAX_CLIENTS 10
#define SERVER_PROJ_ID 'S'

/* Message Types */
typedef enum {
    INIT = 1,
    MESSAGE = 2,
    STOP = 3
} msg_type_t;

/* Message Structure */
typedef struct {
    long mtype;           /* Message type, required by msgsnd/msgrcv */
    int client_id;        /* Assigned client ID */
    key_t client_key;     /* Key of the client's queue (used in INIT) */
    char text[MAX_MSG_LEN]; /* Message body */
} message_t;

/* Helper to get server queue key */
key_t get_server_key() {
    return ftok(getenv("HOME"), SERVER_PROJ_ID);
}

#endif /* COMMON_H */
