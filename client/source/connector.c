#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "../../controller/header/utils.h"
#include "../header/connector.h"

#define PORT 8080

int login_attempts(SOCKET sock) {
    Login_infos login_info;
    int login_status;
    while (1) {
        char username[64];
        printf("Entrez votre identifiant : ");
        scanf("%s", username);

        char password[64];
        printf("Entrez votre mot de passe : ");
        scanf("%s", password);

        strncpy(login_info.username, username, sizeof(login_info.username));
        strncpy(login_info.password, password, sizeof(login_info.password));

        printf("[DEBUG] Envoi des informations de connexion - username: %s, password: %s\n", login_info.username, login_info.password);

        send(sock, (char *)&login_info, sizeof(Login_infos), 0);
        recv(sock, (char *)&login_status, sizeof(int), 0);

        if (login_status == 1) {
            printf("Connexion réussie\n");
            return 0;
        } else {
            printf("Une erreur est survenue lors de la connexion\n");
        }
    }
    return 1;
}

void send_message(Client_data *client, char text[1024]) {
    Message message;
    message.client_id = client->client_id;
    strncpy(message.message, text, sizeof(message.message));
    message.message[sizeof(message.message) - 1] = '\0';
    printf("Infos envoyées : %s %d à %d\n", message.message, message.client_id, client->sock_pointer);
    send(client->sock_pointer, (char *)&message, sizeof(Message), 0);
}

void *receive_messages(void *arg) { // permet de recevoir une notification lorsqu'un message est broadcasté par le serveur
    SOCKET sock = *(SOCKET *)arg;
    printf("socket :%d\n", sock);
    char buffer[1200];
    while (1) {
        int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes > 0) {
            printf("%s", buffer);
        } else {
            printf("haha\n");
            break;
        }
    }
    return NULL;
}

void client_start() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET server_sock;
    struct sockaddr_in server;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);

    connect(server_sock, (struct sockaddr *)&server, sizeof(server));
    if (login_attempts(server_sock) == 1) {
        closesocket(server_sock);
        WSACleanup();
        return;
    }

    Client_data *client = malloc(sizeof(Client_data));

    recv(server_sock, (char *)client, sizeof(Client_data), 0);
    client->sock_pointer = server_sock;

    printf("[DEBUG] Client_data reçu: id=%d pseudo=%s, sock=%d\n", client->client_id, client->client_name, client->sock_pointer);

    SOCKET *sock_copy = malloc(sizeof(SOCKET));
    *sock_copy = server_sock;
    printf("%d", *sock_copy);
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, (void *)sock_copy);

    printf("Tu peux envoyer un message :\n");

    char message[1024];
    while (fgets(message, sizeof(message), stdin)) {
        size_t len = strlen(message);
        if (len == 1) {
            printf("[DEBUG] Message vide, on continue sans envoyer\n");
        } else {
            message[len - 1] = '\0';
            send_message(client, message);
        }
    }

    closesocket(server_sock);
    free(sock_copy);
    free(client);
    WSACleanup();
}