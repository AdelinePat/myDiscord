#ifndef UTILS_H
#define UTILS_H

#define MAX_CLIENTS 20

char *get_timestamp();

typedef struct {
    int client_id;
    struct tm timestamp;
    char message[1024];
} Message;

typedef struct Server_state Server_state;

typedef struct {
    int sock_pointer;
    int client_id;
    Server_state *server;
} Client_data;

struct Server_state {
    Client_data *clients[MAX_CLIENTS];
    int client_count;
    pthread_mutex_t lock;
};

#endif