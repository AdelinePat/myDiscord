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
int connect_to_server(Client_package_for_frontend *login_pack) {
    login_pack->client_package->send_type = SOCKET_OK;

    printf("valeur de send_type %d et valeur de la sock_pointer %lld\n", login_pack->client_package->send_type, login_pack->client_package->client->sock_pointer);

    send(login_pack->client_package->client->sock_pointer,
        (char *)&login_pack->client_package->send_type,
        sizeof(int),
        0);

    recv(login_pack->client_package->client->sock_pointer,
        (char *)&login_pack->client_package->send_type,
        sizeof(int),
        0);

    if (login_pack->client_package->send_type == SOCKET_FAIL) {
        printf("[ERROR] Le serveur renvoie une error\n");
        return 1;
    }
    return 0;


}

int login_attempts(Client_package_for_frontend *login_pack) {
    // Login_infos *login_info_copy = malloc(sizeof(Login_infos));
    // Client_package *client_package_copy = malloc(sizeof(Client_package));
    // client_package_copy->login_info = login_info_copy;
    
    // client_package_copy = login_pack->client_package;
    // client_package_copy->send_type = LOGIN;

    char * client_pack_str = serialize_client_package(login_pack->client_package);
    int login_status = 0;

    // printf("taille de client_package_copy %d\n", sizeof(client_package_copy));
    printf("contenu de l'envoie : %s", client_pack_str);
    // printf("\n\nAttempting login: \n\t%s password :\n\t %s\n\n",
    //     client_package_copy->login_info->username,
    //     client_package_copy->login_info->password);
    int len = strlen(client_pack_str);
    send(login_pack->client_package->client->sock_pointer,
        (char *)&len,
        sizeof(int),
        0);

    send(login_pack->client_package->client->sock_pointer,
        client_pack_str,
        len,
        0);

    recv(login_pack->client_package->client->sock_pointer,
        (char *)&login_status,
        sizeof(int),
        0);

    printf("\n\nLOGIN STATUS APRES RECV %d\n\n", login_status);

    if (login_status == 1) {
        printf("Connexion réussie\n");
        receive_client_data(login_pack);
        // free(login_info_copy);
        // free(client_package_copy);
        return 0;
    } else {
        printf("Une erreur est survenue lors de la connexion\n");
        // free(login_info_copy);
        // free(client_package_copy);
        return 1;
    }
    
}

void receive_client_data(Client_package_for_frontend *login_pack) {
    int socket_client = login_pack->client_package->client->sock_pointer;

    recv(socket_client,
        (char *)login_pack->client_package->client,
        sizeof(Client_data),
        0);

    login_pack->client_package->client->sock_pointer = socket_client; // when receive, socket client is changed, this line is for change it back

    printf("[DEBUG] Client_data reçu: id=%d pseudo=%s, sock=%lld\n",
        login_pack->client_package->client->user_id,
        login_pack->client_package->client->client_name,
        login_pack->client_package->client->sock_pointer);
}

void broadcast_notifications_receiver_start(Client_package_for_frontend *login_pack, GtkWidget *chat_display) {
    SOCKET *sock_copy = malloc(sizeof(SOCKET));
    *sock_copy = login_pack->client_package->client->sock_pointer;
    Chat_display_package *chat_display_package = malloc(sizeof(Chat_display_package));
    chat_display_package->sock_copy = sock_copy;
    chat_display_package->chat_display = chat_display;
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, (void *)chat_display_package);
}

int register_attempts(Client_package_for_frontend *login_pack) {
    Login_infos *login_info_copy = malloc(sizeof(Login_infos));
    *login_info_copy = *login_pack->client_package->login_info;
    int register_status = 1;
    // login_pack->login_info->login_register = CREATE_ACCOUNT;
    send(login_pack->client_package->client->sock_pointer,
        (char *)login_pack->client_package->login_info,
        sizeof(Login_infos),
        0);

    recv(login_pack->client_package->client->sock_pointer, (char *)&register_status, sizeof(int), 0);

    if (login_pack->client_package->send_type == CREATE_ACCOUNT) {
        printf("Création de compte réussie\n");
        return 0;
    } else {
        printf("Une erreur est survenue lors de l'inscription'\n");
        return 1; // in front : go back to login
    }
}

void send_message(Client_package *client_package, char text[1024]) {
    Message message;
    message.user_id = client_package->client->user_id;
    message.channel_id = client_package->current_channel;
    strncpy(message.message, text, sizeof(message.message));
    message.message[sizeof(message.message) - 1] = '\0';
    printf("Infos envoyées : %s de %d à %lld\n", message.message, message.user_id, client_package->client->sock_pointer);
    send(client_package->client->sock_pointer, (char *)&message, sizeof(Message), 0);
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
        printf("BLBLBL received size : %lld\n", size_of_list);
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
    printf("\nla sock client : %lld\n", server_sock);
    return server_sock;
}