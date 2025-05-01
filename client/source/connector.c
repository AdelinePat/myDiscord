// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <pthread.h>
// #include <winsock2.h>
// #include <ws2tcpip.h>
#include "../../controller/header/utils.h"
#include "../header/connector.h"
#include "../header/client_front.h"

#define PORT 8080

int login_attempts(Login_package_for_front *login_pack) {
    Login_infos *login_info_copy = malloc(sizeof(Login_infos));
    *login_info_copy = *login_pack->login_info;
    int login_status = 0;

    // char username[64];
    // printf("Entrez votre identifiant : ");
    // scanf("%s", username);

    // char password[64];
    // printf("Entrez votre mot de passe : ");
    // scanf("%s", password);

    // strncpy(login_info.username, username, sizeof(login_info.username));
    // strncpy(login_info.password, password, sizeof(login_info.password));


    // printf("[DEBUG] Envoi des informations de connexion - username: %s, password: %s\n", login_info.username, login_info.password);
    printf("Avant send : valeur de login_status : %d\n", login_status);
    printf("Attempting login: %s / %s\n", login_pack->login_info->username, login_pack->login_info->password);
    send(login_pack->client->sock_pointer, (char *)login_info_copy, sizeof(Login_infos), 0);
    recv(login_pack->client->sock_pointer, (char *)&login_status, sizeof(int), 0);
    printf("Après receive : valeur de login_status : %d\n", login_status);

    if (login_status == 1) {
        printf("Connexion réussie\n");
        receive_client_data(login_pack);
        return 0;
    } else {
        printf("Une erreur est survenue lors de la connexion\n");
        return 1;
    }
}

void receive_client_data(Login_package_for_front *login_pack) {
    int socket_client = login_pack->client->sock_pointer;

    recv(socket_client, (char *)login_pack->client, sizeof(Client_data), 0);
    login_pack->client->sock_pointer = socket_client; // when receive, socket client is changed, this line is for change it back

    printf("[DEBUG] Client_data reçu: id=%d pseudo=%s, sock=%d\n", login_pack->client->client_id, login_pack->client->client_name, login_pack->client->sock_pointer);

    broadcast_notifications_receiver_start(login_pack);
}

void broadcast_notifications_receiver_start(Login_package_for_front *login_pack) {
    SOCKET *sock_copy = malloc(sizeof(SOCKET));
    *sock_copy = login_pack->client->sock_pointer;
    printf("%lld", *sock_copy);
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, (void *)sock_copy);
}

int register_attempts(Login_package_for_front *login_pack) {
    int register_status = 1;
    send(login_pack->client->sock_pointer, (char *)login_pack->login_info, sizeof(Login_infos), 0);
    recv(login_pack->client->sock_pointer, (char *)&register_status, sizeof(int), 0);

    if (register_status == 1) {
        printf("Création de compte réussie\n");
        return 0;
    } else {
        printf("Une erreur est survenue lors de l'inscription'\n");
        return 1; // in front : go back to login
    }
}

void send_message(Client_data *client, char text[1024]) {
    Message message;
    message.client_id = client->client_id;
    strncpy(message.message, text, sizeof(message.message));
    message.message[sizeof(message.message) - 1] = '\0';
    printf("Infos envoyées : %s de %d à %lld\n", message.message, message.client_id, client->sock_pointer);
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
    free(arg);
    return NULL;
}

SOCKET client_start() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET server_sock;
    struct sockaddr_in server;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);

    connect(server_sock, (struct sockaddr *)&server, sizeof(server));
    printf("la sock serveur : %lld", server_sock);
    return server_sock;
    }

// void after_login()
//     {
//     if (login_attempts(server_sock) == 1) {
//         closesocket(server_sock);
//         WSACleanup();
//         return;
//     }

//     Client_data *client = malloc(sizeof(Client_data)) free();

//     recv(server_sock, (char *)client, sizeof(Client_data), 0);
//     client->sock_pointer = server_sock;

//     printf("[DEBUG] Client_data reçu: id=%d pseudo=%s, sock=%d\n", client->client_id, client->client_name, client->sock_pointer);

//     SOCKET *sock_copy = malloc(sizeof(SOCKET)) free();
//     *sock_copy = server_sock;
//     printf("%d", *sock_copy);
//     pthread_t recv_thread;
//     pthread_create(&recv_thread, NULL, receive_messages, (void *)sock_copy);

//     printf("Tu peux envoyer un message :\n");

//     char message[1024];
//     while (fgets(message, sizeof(message), stdin)) {
//         size_t len = strlen(message);
//         if (len == 1) {
//             printf("[DEBUG] Message vide, on continue sans envoyer\n");
//         } else {
//             message[len - 1] = '\0';
//             send_message(client, message);
//         }
//     }

//     closesocket(server_sock);
//     free(sock_copy);
//     free(client);
//     WSACleanup();
// }