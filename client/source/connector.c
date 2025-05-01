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

    printf("[DEBUG] Client_data reçu: id=%d pseudo=%s, sock=%lld\n", login_pack->client->client_id, login_pack->client->client_name, login_pack->client->sock_pointer);
}

void broadcast_notifications_receiver_start(Login_package_for_front *login_pack, GtkWidget *chat_display) {
    SOCKET *sock_copy = malloc(sizeof(SOCKET));
    *sock_copy = login_pack->client->sock_pointer;
    Chat_display_package *chat_display_package = malloc(sizeof(Chat_display_package));
    chat_display_package->sock_copy = sock_copy;
    chat_display_package->chat_display = chat_display;
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, (void *)chat_display_package);
}

int register_attempts(Login_package_for_front *login_pack) {
    Login_infos *login_info_copy = malloc(sizeof(Login_infos));
    *login_info_copy = *login_pack->login_info;
    int register_status = 1;
    login_pack->login_info->login_register = CREATE_ACCOUNT;
    send(login_pack->client->sock_pointer, (char *)login_pack->login_info, sizeof(Login_infos), 0);
    recv(login_pack->client->sock_pointer, (char *)&register_status, sizeof(int), 0);

    if (login_pack->login_info->login_register == CREATE_ACCOUNT) {
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

void fill_in_chat(GtkWidget *chat_display, Message *message_list, int message_length) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(chat_display));
        GtkTextIter end_iter;
        gtk_text_buffer_get_end_iter(buffer, &end_iter);
        for (int i = 0; i < message_length; i++) {
            gchar text[150];
            snprintf(text, sizeof(text), "[%s] : %s", message_list[i].username, message_list[i].message);
            // printf("Valeur row num%d : message_id %d, user_id %d, username %s, content %s\n", i, message_list[i].message_id, message_list[i].client_id, message_list[i].username, message_list[i].message);
            gtk_text_buffer_insert(buffer, &end_iter, text, -1);
            gtk_text_buffer_insert(buffer, &end_iter, "\n", -1);
        }
}

void *receive_messages(void *arg) { // permet de recevoir une notification lorsqu'un message est broadcasté par le serveur
    Chat_display_package *chat_display_package = (Chat_display_package *)arg;
    GtkWidget *chat_display = chat_display_package->chat_display;
    while (1) {
        recover_messages(*chat_display_package->sock_copy, chat_display);
    }
    free(arg);
    return NULL;
}

void recover_messages(SOCKET sock, GtkWidget *chat_display) {
    printf("socket :%lld\n", sock);
    int size_of_list;
    int bytes = recv(sock, (char *)&size_of_list, sizeof(int), 0);
    if (bytes > 0) {
        printf("received size : %lld\n", size_of_list);
    }
    if (bytes == SOCKET_ERROR)
    {
        printf("[ERROR] Erreur lors de la réception de la taille d'array, code %d\n", WSAGetLastError());
    }
    if (bytes != sizeof(int))
    {
        printf("[ERROR] Taille des données size_of_list reçues incorrecte. Attendu %zu, reçu %d\n", sizeof(size_t), bytes);
    }
    int validation = 1;
    send(sock, (char *)validation, sizeof(int), 0);
    Message *message_list = malloc(sizeof(Message) * size_of_list);
    for (long long unsigned int i = 0; i < size_of_list; i++) {
        Message new_message;
        bytes = recv(sock, (char *)&new_message, sizeof(Message), 0);
        if (bytes > 0) {
            printf("hihi");
        }
        if (bytes == SOCKET_ERROR)
        {
            printf("[ERROR] Erreur lors de la réception du message : %lld, code %d\n", i, WSAGetLastError());
        }
        if (bytes != sizeof(Message))
        {
            printf("[ERROR] Taille des données reçues incorrecte. Attendu %zu, reçu %d\n", sizeof(Message), bytes);
        }
        message_list[i] = new_message;
        printf("Message ajouté à la liste\n");
        send(sock, (char *)validation, sizeof(int), 0);
        printf("Validation envoyée\n");
    }
    fill_in_chat(chat_display, message_list, size_of_list);
    free(message_list);
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