#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "../common/utils.h"

#define PORT 8080

void send_message(int sock, int client_id, const char *text);
void *receive_messages(void *arg);

void client_start() {
    int sock;
    struct sockaddr_in server;
    char message[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);

    Client_data *client;

    connect(sock, (struct sockaddr *)&server, sizeof(server));
    recv(sock, &client, sizeof(int), 0);
    int id = client->client_id;

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, &sock);

    printf("Connecté au serveur ! Tu peux écrire maintenant :\n");

    while (fgets(message, sizeof(message), stdin)) {
        send_message(sock, id, message);
    }

    close(sock);
}

void send_message(int sock, int client_id, const char *text) {
    Message msg;
    msg.client_id = client_id;

    strncpy(msg.message, text, sizeof(msg.message));
    msg.message[sizeof(msg.message) - 1] = '\0';

    send(sock, &msg, sizeof(Message), 0);
}

void *receive_messages(void *arg) {
    int sock = *(int *)arg;
    char buffer[1024];
    while (1) {
        int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            printf("%s", buffer);
        } else {break;}
    }
    return NULL;
}