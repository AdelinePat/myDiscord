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
    printf("dans login_info\n");
    // Login_infos *login_info_copy = malloc(sizeof(Login_infos));
    // Client_package *client_package_copy = malloc(sizeof(Client_package));
    // client_package_copy->login_info = login_info_copy;
    
    // client_package_copy = login_pack->client_package;
    // client_package_copy->send_type = LOGIN;
    login_pack->client_package->current_channel = 1;
    char * client_pack_str = serialize_client_package(login_pack->client_package);
    int login_status = 0;
    printf("\n\nclient_pack_str = \n%s\n", client_pack_str);
    // printf("taille de client_package_copy %d\n", sizeof(client_package_copy));
    // printf("contenu de l'envoie : %s", client_pack_str);
    // printf("\n\nAttempting login: \n\t%s password :\n\t %s\n\n",
    //     client_package_copy->login_info->username,
    //     client_package_copy->login_info->password);
    int len = strlen(client_pack_str);
    printf("len de client_pack_str = %d", len);
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
        printf("Connexion réussie login_attempts client side\n");
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

    int handshake = READY;
    // sending a ready message to server
    char time_stamp[LARGE_PLUS_STRING];
    debug_get_str_timestamp(time_stamp, LARGE_PLUS_STRING);
    printf("\n[RECEIVE CLIENT DATA]before handshake timestamp \t%s\n", time_stamp);
    send(login_pack->client_package->client->sock_pointer,
        (char *)&handshake,
        sizeof(int),
        0);

    int request = CLIENT_RECV;
    
    send(login_pack->client_package->client->sock_pointer,
        (char *)&request,
        sizeof(int),
        0);

    // recv(socket_client,
    //     (char *)login_pack->client_package->client,
    //     sizeof(Client_data),
    //     0);
    fill_in_structures(login_pack);
    // recover_messages(login_pack);
    // login_pack->client_package->client->sock_pointer = socket_client; // when receive, socket client is changed, this line is for change it back

    printf("[DEBUG] Client_data reçu: id=%d pseudo=%s, sock=%lld\n",
        login_pack->client_package->client->user_id,
        login_pack->client_package->client->username,
        login_pack->client_package->client->sock_pointer);
}

// void broadcast_notifications_receiver_start(Client_package_for_frontend *login_pack, GtkWidget *chat_display) {
//     SOCKET *sock_copy = malloc(sizeof(SOCKET));
//     *sock_copy = login_pack->client_package->client->sock_pointer;
//     Chat_display_package *chat_display_package = malloc(sizeof(Chat_display_package));
//     chat_display_package->sock_copy = sock_copy;
//     chat_display_package->chat_display = chat_display;
//     pthread_t recv_thread;
//     pthread_create(&recv_thread, NULL, receive_messages, (void *)chat_display_package);
// }

void broadcast_notifications_receiver_start(Client_package_for_frontend *login_pack) {
    // SOCKET *sock_copy = malloc(sizeof(SOCKET));
    // *sock_copy = login_pack->client_package->client->sock_pointer;
    // Chat_display_package *chat_display_package = malloc(sizeof(Chat_display_package));
    // chat_display_package->sock_copy = sock_copy;
    // chat_display_package->chat_display = chat_display;
    printf("valeur de send_type dans broadcast_notifications %d\n",
        login_pack->client_package->send_type);
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, (void *)login_pack);
}

int register_attempts(Client_package_for_frontend *login_pack) {
    int handshake = READY;
    // sending a ready message to server
    char time_stamp[LARGE_PLUS_STRING];
    debug_get_str_timestamp(time_stamp, LARGE_PLUS_STRING);
    printf("\n[RECOVER MSG] before handshake timestamp \t%s\n", time_stamp);
    send(login_pack->client_package->client->sock_pointer,
        (char *)&handshake,
        sizeof(int),
        0);
    
    int request = CLIENT_SEND;
    send(login_pack->client_package->client->sock_pointer,
        (char *)&request,
        sizeof(int),
        0);

    

    Login_infos *login_info_copy = malloc(sizeof(Login_infos));
    *login_info_copy = *login_pack->client_package->login_info;
    int register_status = 1;
    char *json_string = serialize_client_package(login_pack->client_package);
    int str_len = strlen(json_string);
    debug_get_str_timestamp(time_stamp, LARGE_PLUS_STRING);
    printf("\n\t\t[TIME BEFORE SEND LEN] register_attempts timestamp \t%s\n", time_stamp);
    send(login_pack->client_package->client->sock_pointer,
        (char *)&str_len,
        sizeof(int), 0);
    printf("registr attempts envoie de la longueur de la string dans send message %d\n", str_len);

    send(login_pack->client_package->client->sock_pointer,
        json_string,
        str_len, 0);

    int validation = 1;
    send(login_pack->client_package->client->sock_pointer,
        (char *)validation,
        sizeof(int),
        0);

    if (validation == 1) {
        register_status = 0;
    }
    return register_status;
    // login_pack->login_info->login_register = CREATE_ACCOUNT;
    // send(login_pack->client_package->client->sock_pointer,
    //     (char *)login_pack->client_package->login_info,
    //     sizeof(Login_infos),
    //     0);

    // recv(login_pack->client_package->client->sock_pointer, (char *)&register_status, sizeof(int), 0);

    // if (login_pack->client_package->send_type == CREATE_ACCOUNT) {
    //     printf("Création de compte réussie\n");
    //     return 0;
    // } else {
    //     printf("Une erreur est survenue lors de l'inscription'\n");
    //     return 1; // in front : go back to login
    // }
}

void send_message(Client_package *client_package, char text[1024]) {
    Message message;
    message.user_id = client_package->client->user_id;
    message.channel_id = client_package->current_channel;
    strncpy(message.username, client_package->client->username, SMALL_STRING);
    strncpy(message.message, text, sizeof(message.message));
    message.message[sizeof(message.message) - 1] = '\0';
    printf("je rajoute le message au client_package\n\n");
    client_package->message_send = message;

    client_package->send_type = SEND_MESSAGE;
    // int handshake = READY;
    // // sending a ready message to server
    // char time_stamp[LARGE_PLUS_STRING];
    // debug_get_str_timestamp(time_stamp, LARGE_PLUS_STRING);
    // printf("\n[RECEIVE CLIENT DATA]before handshake timestamp \t%s\n", time_stamp);
    // send(client_package->client->sock_pointer,
    //     (char *)&handshake,
    //     sizeof(int),
    //     0);

    // int request = CLIENT_SEND;
    // send(client_package->client->sock_pointer,
    //     (char *)&request,
    //     sizeof(int),
    //     0);

    // printf("Infos envoyées : %s de %d à %lld\n", client_package->message_send.message, client_package->message_send.user_id, client_package->client->sock_pointer);
    // printf("avant le serialize dans send message\n\n");

    // char *json_string = serialize_client_package(client_package);
    // int str_len = strlen(json_string);
    
    // debug_get_str_timestamp(time_stamp, LARGE_PLUS_STRING);
    // printf("\n\t\t[TIME BEFORE SEND LEN] send_message timestamp \t%s\n", time_stamp);
    // send(client_package->client->sock_pointer,
    //     (char *)&str_len,
    //     sizeof(int), 0);
    // printf("envoie de la longueur de la string dans send message %d\n", str_len);

    // send(client_package->client->sock_pointer,
    //     json_string,
    //     str_len, 0);
    // printf("envoie de la string ds send message\n\n");
    // send(client_package->client->sock_pointer, (char *)&message, sizeof(Message), 0);
}

void fill_in_chat(GtkWidget *chat_display, Message *message_list, int message_length) {
    printf("je suis dans fill_in_chat");
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(chat_display));
    for (int i = 0; i < message_length; i++) {
        GtkTextIter end_iter;
        gtk_text_buffer_get_end_iter(buffer, &end_iter);
        gchar text[150];
        snprintf(text, sizeof(text), "%s    %s : \n%s\n",
                 message_list[i].username,
                 message_list[i].timestamp,
                 message_list[i].message);
        gtk_text_buffer_insert(buffer, &end_iter, text, -1);
        gtk_text_buffer_insert(buffer, &end_iter, "\n", -1);
    }
    
}

void clear_chat(GtkWidget *chat_display) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(chat_display));
    gtk_text_buffer_set_text(buffer, "", -1);
    // printf("inside clear_chat ??\n");
    // GList *children, *iter;
    // children = gtk_container_get_children(GTK_CONTAINER(chat_display));
    // for (iter = children; iter != NULL; iter = g_list_next(iter)) {
    //     gtk_widget_destroy(GTK_WIDGET(iter->data));
    //     printf("\ndans clear_chat iteration");
    // }
    // g_list_free(children);
}

void *receive_messages(void *arg) { // permet de recevoir une notification lorsqu'un message est broadcasté par le serveur
    // Chat_display_package *chat_display_package = (Chat_display_package *)arg;
    printf("inside receive message that calls for a while(1) and calls continously for recover_messages\n\n");
    Client_package_for_frontend *client_pack = (Client_package_for_frontend *)arg;
    GtkWidget *chat_display = client_pack->chat_display;
    while (1) {
        // recover_messages(*chat_display_package->sock_copy, chat_display);
        if (client_pack->client_package->send_type != MESSAGE_REFRESHED) {
            printf("\navant recover message dans while(1) receive messages\n");
            recover_messages(client_pack);
            printf("\n\n\n\tAAAAAAAAAAAAAAAAAAAAAAAAAAAAAH\navant gtk_widget_show_all\n\n\n");
            gtk_widget_show_all(client_pack->window);
            printf("\naprès gtl_widget_show_all\n");
        }
        
    }
    free(arg);
    return NULL;
}

void fill_in_structures(Client_package_for_frontend *login_pack) {
    printf("\njuste avant reception str_len : socket :%lld\n", login_pack->client_package->client->sock_pointer);
    // sending a ready message to server
    char time_stamp[LARGE_PLUS_STRING];

    int str_len;
    debug_get_str_timestamp(time_stamp, LARGE_PLUS_STRING);
    printf("\n[TIME BEFORE RECV LEN] timestamp \t%s\n", time_stamp);
    int bytes = recv(login_pack->client_package->client->sock_pointer,
        (char *)&str_len,
        sizeof(int),
        0);

    
    debug_get_str_timestamp(time_stamp, LARGE_PLUS_STRING);
    printf("\n[TIME AFTER RECV LEN] timestamp \t%s\n", time_stamp);
    // printf("\n est ce que la len est le debut de la string ?? %s", str_len);

    // Convert from network byte order to host byte order
    // str_len = ntohl(str_len);
    
    if (bytes > 0 && bytes == sizeof(int)) {
        int str_len_network = ntohl(str_len);
        printf("BLBLBL received size : %d, converted ntohl %d\n", str_len, str_len_network);
    }
    if (bytes == SOCKET_ERROR)
    {
        printf("[ERROR] Erreur lors de la réception de la taille d'array, code %d\n", WSAGetLastError());
    }
    if (bytes != sizeof(int))
    {
        printf("[ERROR] Taille des données size_of_list reçues incorrecte. Attendu %zu, reçu %d\n", sizeof(int), bytes);
    }

    int validation = 1;
    char * client_package_str = malloc(str_len + 1);

    int json_bytes = recv(login_pack->client_package->client->sock_pointer,
        client_package_str,
        str_len,
        0);
    
    client_package_str[str_len] = '\0';
    printf("j'imprime la reception de client_package_str \n%s\n", client_package_str);

    parse_client_package_str(login_pack->client_package, client_package_str);
    
    // printf("CHECKPOINT: login_info ptr = %p\n", login_pack->client_package->login_info);
    printf("[dans recover message] CHECKPOINT: login_info ptr = %p\n channels %p\n", login_pack->client_package->login_info, login_pack->client_package->channels);
    
    
    // printf("\n\n\nréception de client_package dans recover_messages : %s\n\n\n", client_package_str);
    free(client_package_str);

    send(login_pack->client_package->client->sock_pointer,
        (char *)validation,
        sizeof(int),
        0);
}

void recover_messages(Client_package_for_frontend *login_pack) {
    int handshake = READY;
    // sending a ready message to server
    char time_stamp[LARGE_PLUS_STRING];
    debug_get_str_timestamp(time_stamp, LARGE_PLUS_STRING);
    printf("\n[RECOVER MSG] before handshake timestamp \t%s\n", time_stamp);
    send(login_pack->client_package->client->sock_pointer,
        (char *)&handshake,
        sizeof(int),
        0);
    
    int request = -1;
    if (login_pack->client_package->send_type == SEND_MESSAGE) {
        request = CLIENT_SEND;
    } else {
        request = CLIENT_RECV;
    }
    printf("\n\nvaleur de request dans recover message : %d\n\n", request);
    
    send(login_pack->client_package->client->sock_pointer,
        (char *)&request,
        sizeof(int),
        0);
    if (request == CLIENT_SEND) {
        char *json_string = serialize_client_package(login_pack->client_package);
        int str_len = strlen(json_string);
        debug_get_str_timestamp(time_stamp, LARGE_PLUS_STRING);
        printf("\n\t\t[TIME BEFORE SEND LEN] send_message timestamp \t%s\n", time_stamp);
        send(login_pack->client_package->client->sock_pointer,
            (char *)&str_len,
            sizeof(int), 0);
        printf("envoie de la longueur de la string dans send message %d\n", str_len);
    
        send(login_pack->client_package->client->sock_pointer,
            json_string,
            str_len, 0);

        int validation = 1;
        send(login_pack->client_package->client->sock_pointer,
            (char *)validation,
            sizeof(int),
            0);
    }

    if (request == CLIENT_SEND) {
        int handshake = READY;
    // sending a ready message to server
        char time_stamp[LARGE_PLUS_STRING];
        debug_get_str_timestamp(time_stamp, LARGE_PLUS_STRING);
        printf("\n[RECOVER MSG] avant fill_in_struct before handshake timestamp \t%s\n", time_stamp);
        send(login_pack->client_package->client->sock_pointer,
            (char *)&handshake,
            sizeof(int),
            0);
        request = CLIENT_RECV;
    
        printf("avant : fill in structure dans recover_message");
        fill_in_structures(login_pack);
    }
    

    
    
    

    // if (login_pack->client_package->send_type != LOGIN &&
    //     login_pack->client_package->send_type != CREATE_ACCOUNT) {
        printf("before clear_chat\n");
        // fill_in_chat(login_pack->chat_display, message_list, size_of_list);
        clear_chat(login_pack->chat_display);
        printf("before fill_in_chat\n");
        fill_in_chat(login_pack->chat_display,
            login_pack->client_package->messages_list,
            login_pack->client_package->number_of_messages);
        login_pack->client_package->send_type = MESSAGE_REFRESHED;
        printf("after fill_in_chat\n");
        // free(message_list);
    // }
    printf("\n\nvaleur de request dans fin recover message : %d\n\n", request);
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