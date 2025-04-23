#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "handler.h"

#define PORT 8080

void start_server() {
    Server_state *state = malloc(sizeof(Server_state));
    state->client_count = 0;
    pthread_mutex_init(&state->lock, NULL);
    int server_fd, new_sock;
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Erreur bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Erreur listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Serveur en écoute sur le port %d\n", PORT);

    while ((new_sock = accept(server_fd, (struct sockaddr *)&client, &c))) {
        pthread_t thread;
        Client_data *client_data = malloc(sizeof(Client_data)):
        int *sock_ptr = malloc(sizeof(int));
        *sock_pointer = new_sock;
        client_data->sock_ptr = sock_ptr;
        client_data->id = state->client_count++;
        client_data->server = state;
        pthread_create(&thread, NULL, handle_client, client_data);
        pthread_detach(thread);
    }
}

void *handle_client(void *arg) {
    Client_data *client = (Client_data *)arg;
    Server_state *state = client->server;
    int sock = client->sock_pointer;
    int id = client->id;

    send(sock, &client, sizeof(Client_data), 0);

    pthread_mutex_lock(&state->lock);
    state->clients[id] = client;
    pthread_mutex_unlock(&state->lock);

    printf("[INFO] Client %d connecté.\n", id);

    while (1) {
        int bytes = recv(sock, client_message, sizeof(Message), 0);
        if (bytes <= 0) break;

        buffer[bytes] = '\0';

        char *time_now = get_timestamp();
        client_message->timestamp = time_now;

        broadcast_message(state, client_message);
    }

    printf("[INFO] Client %d déconnecté.\n", id);
    close(sock);

    pthread_mutex_lock(&state->lock);
    for (int i = 0; i < state->client_count; i++) {
        if (clients[i] == client) {
            free(clients[i])
            for (int j = i; j < state->client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            state->client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&state->lock);

    free(arg);
    pthread_exit(NULL);
}

void broadcast_message(Server_state *state, Message client_message) {
    char formatted[1200];
    char time_str[64];

    strftime(time_str, sizeof(time_str), "[%H:%M:%S]", &client_message->timestamp);
    snprintf(formatted, sizeof(formatted), "%s [Client %d] %s", time_str, client_message->client_id, client_message->message);

    pthread_mutex_lock(&state->lock);
    for (int i = 0; i < state->client_count; i++) {
        send(state->clients[i]->socket, formatted, strlen(formatted), 0);
    }
    pthread_mutex_unlock(&state->lock);
    free(client_message->timestamp);
}