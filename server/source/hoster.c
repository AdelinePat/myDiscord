#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <winsock2.h>
#include <ws2tcpip.h>
// #pragma comment(lib, "ws2_32.lib")

// #include "../controller/header/utils.h"
#include "../header/hoster.h"
#include "../header/database_communication.h"
#include "../header/db_connection.h"
#include "../header/db_chat_content.h"
#include "../header/register_user.h"


#define PORT 8080

int connect_to_client(Client_package *client_package) {
    
    recv(client_package->client->sock_pointer,
        (char *)&client_package->send_type,
        sizeof(client_package->send_type),
        0);
    
    printf("valeur de send_type %d et valeur de la sock_pointer serveur %lld\n",
        client_package->send_type,
        client_package->client->sock_pointer);

    if (client_package->send_type == SOCKET_OK) {
        printf("je renvoie socket_ok\n");
        send(client_package->client->sock_pointer,
            (char *)&client_package->send_type,
            sizeof(client_package->send_type),
            0);
        return 0;
    } else {
        printf("je renvoie socket_fail\n");
        client_package->send_type = SOCKET_FAIL;
            send(client_package->client->sock_pointer,
            (char *)&client_package->send_type,
            sizeof(client_package->send_type),
            0);
        return 1;
    }
}

int login_attempts(Client_package *client_package)
{
    while (1)
    {
        int login_status = 0;
        int len = 0;
        int check_len = recv(client_package->client->sock_pointer,
            (char *)&len,
            sizeof(int),
            0);

        if (check_len == SOCKET_ERROR) {
            printf("[ERROR] Failed socket connexion from client to server\n");
        }

        if (check_len != sizeof(int))
        {
            printf("[ERROR] Failed to receive message length. Received %d bytes\n", check_len);
            break;
        }

        printf("\n\n\n\n\n\t\t\t len %d\n", len);

        char * client_package_str = malloc(len + 1);

        printf("\t\t\tla longueur du json est de %d", (int)len);

        int bytes = recv(client_package->client->sock_pointer,
            client_package_str,
            len,
            0);
        if (bytes == SOCKET_ERROR)
        {
            printf("[ERROR] Erreur lors de la réception de Login_infos, code %d\n", WSAGetLastError());
            break;
        }
        if (bytes != len)
        {
            printf("[ERROR] Taille des données reçues incorrecte. Attendu %zu, reçu %d\n", len, bytes);
            break;
        }

        client_package_str[len] = '\0';
        printf("\n\nvaleur du json \t%s\n\n", client_package_str);

        // cJSON *root = cJSON_Parse(client_package_str);
        parse_client_package_str(client_package, client_package_str);
        // printf("[login_attempts] CHECKPOINT: login_info ptr = %p\n", client_package->login_info);
        printf("\n\n\nPRINT DE SEND_TYPE APRES PARSING DANS LOGIN ATTEMPT : %d\n\n\n", client_package->send_type);
        printf("[login_attempts] CHECKPOINT: login_info ptr = %p\n channels %p\n", client_package->login_info, client_package->channels);
        printf("j'ai fini de récupéré les infos du json\n");
        free(client_package_str);

        printf("Infos reçues du json: user : << %s >>, pass : << %s >>\n",
            client_package->login_info->username,
            client_package->login_info->password);

        printf("\navant le if : Valeur de login_status cote serveur : %d\n", login_status);
        printf("\nvaleur du send_type dans client_package : %d\n", client_package->send_type);
        
        switch (client_package->send_type)
        {
            case CREATE_ACCOUNT:
                create_new_user_db(client_package);
                // break;
            case LOGIN:
                if (check_user_pass_match(client_package->login_info) == 1)
                {
                    login_status = 1;
                    
                    send(client_package->client->sock_pointer, (char *)&login_status, sizeof(login_status), 0);
                    printf("Après send : Valeur de login_status cote serveur : %d\n", login_status);
                    printf("Connexion réussie\n");
                    return 0; // remplacer le return avec l'id du client dans la db
                }
                else
                {
                    login_status = 0;
                    printf("Informations de connexion erronées.\n");
                    send(client_package->client->sock_pointer, (char *)&login_status, sizeof(login_status), 0);
                }
                break;
            default:
                login_status = 0;
                printf("oups, le send_type n'est pas reconnu");
                send(client_package->client->sock_pointer, (char *)&login_status, sizeof(login_status), 0);

        }
        
    }
    
    return 1;
}

void recover_messages(Client_package_for_backend *package)
{
    printf("dans recover_message cote serveur :p juste avant get_full_chat_content\n");
    if (package->client_package->send_type != LOGIN && package->client_package->send_type != CREATE_ACCOUNT)
    {
        printf("\n[recover_messages apres condition avant get_full_chat_content] CHECKPOINT:\n\
            channels ptr = %p\n\
            login_info ptr = %p\n\
            client_package ptr = %p\n\n",
            package->client_package->channels,
            package->client_package->login_info,
            package->client_package);
        
        get_full_chat_content(package->client_package);
        printf("[recover_messages after get_full_chat_content] CHECKPOINT: channels %p\n", package->client_package->channels);
    }
    // printf("DANS recover_message : sock du client : %lld\n", package->client_package->client->sock_pointer);

    int size_of_list = package->client_package->number_of_messages;
    
    printf("message_list size (number of messages) : %d\n", size_of_list);

    // CHANGE TYPE TO SEND !!
    // memset(package->client_package->login_info, 0, sizeof(Login_infos));

    if (!package->client_package || !package->client_package->client || !package->client_package->login_info) {
        fprintf(stderr, "serialize_client_package: NULL field detected\n");
        // return NULL;
    }

    // printf("[recover_messages avant serialize_client_package] CHECKPOINT: client_package ptr = %p\n", package->client_package);
    // printf("[recover_messages avant serialize_client_package] CHECKPOINT: login_info ptr = %p\n\n", package->client_package->login_info);
    // printf("[recover_messages avant serialize_client_package] CHECKPOINT: channels ptr = %p\n", package->client_package->channels);
    // printf("[recover message juste avant serialize_client_package] CHECKPOINT: login_info ptr = %p\n channels %p\n", package->client_package->login_info, package->client_package->channels);
    char * client_pack_str = serialize_client_package(package->client_package);
    // char client_pack_string_final[strlen(package->client_package->client_pack_str)]
    printf("\n\nVoici la string du json a envoyé dans recover_message : \n\n%s\n\n\n",
        client_pack_str);

    // printf("\n\nVoici la string du json a envoyé dans recover_message package->client_package->client_pack_str: \n\n%s\n\n\n",
    //     package->client_package->client_pack_str);
    char time_stamp[LARGE_PLUS_STRING];
    int handshake;
    do {
        debug_get_str_timestamp(time_stamp, LARGE_PLUS_STRING);
        printf("\n[TIME BEFORE RECV WHILE] timestamp \t%s\n", time_stamp);
        recv(package->client_package->client->sock_pointer,
        (char *)&handshake,
        sizeof(int),
        0);
    } while (handshake != READY);
    
    int request;
    recv(package->client_package->client->sock_pointer,
        (char *)&request,
        sizeof(int),
        0);

    printf("sortie de la boucle while, valeur de handshake : %d", handshake);
    
    // if () {
    //     fprintf(stderr, "client not ready!\n");
    // }

    int len = 0;
    // printf("\n\network_len dans recover_message %d\n\n", network_len);
    // char time_stamp[LARGE_PLUS_STRING];
    debug_get_str_timestamp(time_stamp, LARGE_PLUS_STRING);
    printf("\n[TIME BEFORE SEND LEN] timestamp \t%s\n", time_stamp);
    
    switch (request) {
        case CLIENT_SEND:
            printf("reception de la string cote serveur\n");
            debug_get_str_timestamp(time_stamp, LARGE_PLUS_STRING);
            printf("\n[TIME BEFORE RECV LEN] timestamp \t%s\n", time_stamp);
            int bytes = recv(package->client_package->client->sock_pointer,
                (char *)&len, sizeof(int),
                0);

            if (bytes > 0 && bytes == sizeof(int)) {
                int str_len_network = ntohl(len);
                printf("SERVER received size : %d, converted ntohl %d\n", len, str_len_network);
            }
            if (bytes == SOCKET_ERROR)
            {
                printf("[ERROR] Erreur lors de la réception de la taille d'array, code %d\n", WSAGetLastError());
            }
            if (bytes != sizeof(int))
            {
                printf("[ERROR] Taille des données size_of_list reçues incorrecte. Attendu %zu, reçu %d\n", sizeof(int), bytes);
            }

            char * client_package_str = malloc(len + 1);
            int json_bytes = recv(package->client_package->client->sock_pointer,
                client_package_str, len,
                0);
            client_package_str[len] = '\0';

            printf("Raw bytes received (%d):\n", len);
            for (int i = 0; i < len; i++) {
                unsigned char c = client_package_str[i];
                printf("%02x ", c);
            }
            printf("\n");

            printf("Printable view:\n");
            for (int i = 0; i < len; i++) {
                unsigned char c = client_package_str[i];
                printf("%c", (c >= 32 && c <= 126) ? c : '.');
            }
            printf("\n");

            // printf("\nj'imprime la reception de client_package_str \n%s\n", client_package_str);
            parse_client_package_str(package->client_package, client_package_str);
            switch (package->client_package->send_type)
            {
                case SEND_MESSAGE:
                

                char * msg_timestamp = get_str_timestamp();
                strncpy(package->client_package->message_send.timestamp, msg_timestamp, MEDIUM_STRING);
                create_new_message_db(package->client_package);
                get_full_chat_content(package->client_package);
                request = CLIENT_RECV;
            }
            printf("apres parsing\n");
            break;
            // save_message_in_db();
        case CLIENT_RECV:
            printf("\n\n\nDANS SWITCH CASE CLIENT RECV\n\n\n");
            char * client_pack_str = serialize_client_package(package->client_package);
            len = strlen(client_pack_str);
            // Convert to network byte order before sending
            // int network_len = htonl(len);
            printf("\n\nlen dans recover_message %d\n et son adresse %p\n", len, &len);
            send(package->client_package->client->sock_pointer,
            (char *)&len,
            sizeof(int),
            0);
            
            debug_get_str_timestamp(time_stamp, LARGE_PLUS_STRING);
            printf("\n[TIME AFTER SEND LEN] timestamp \t%s\n", time_stamp);
            send(package->client_package->client->sock_pointer,
            client_pack_str,
            len,
            0);
            int validation;
            recv(package->client_package->client->sock_pointer, (char *)validation, sizeof(int), 0);

            printf("sending messages\n");
            printf("messages number : %d\n", package->client_package->number_of_messages);
            printf("       messages sent\n");

    }
    
}

// void broadcast_message(Client_package *client_package)
// {
//     get_full_chat_content(client_package);
//     pthread_mutex_lock(&client_package->server->lock);
//     for (int i = 0; i < client_package->server->client_count; i++) {
//         recover_messages(client_package->server->clients[i]);
//     }
//     pthread_mutex_unlock(&client_package->server->lock);
// }

void *handle_client(void *arg)
{
    Client_package_for_backend *client_pack = (Client_package_for_backend *)arg;
    if (!client_pack || !client_pack->client_package || !client_pack->client_package->login_info) {
        fprintf(stderr, "[handle_client] Client package or login_info is NULL!\n");
    }

    printf("handle_client: login_info pointer = %p\n", client_pack->client_package->login_info);

    printf("\n\nvaleur de la socket server avant connect_to_client : %lld\n\n",
    client_pack->client_package->client->sock_pointer);

    int connect_status = connect_to_client(client_pack->client_package);
    if (connect_status == 1)
    {
        printf("[INFO] dans handle client : Echec du hand-shake");
        free(client_pack->client_package->client);
        free(client_pack->client_package);
        free(client_pack);
        pthread_exit(NULL);
        return 0;
    }

    Server_state *state = client_pack->server;
    Client_data *client = client_pack->client_package->client;
    SOCKET client_sock = client->sock_pointer;

    int login_status = login_attempts(client_pack->client_package);
    // printf("[handle_client after login_attempts] CHECKPOINT: login_info ptr = %p\n", client_pack->client_package->login_info);
    printf("\n[handle_client after login_attempts] CHECKPOINT: login_info ptr = %p\n channels %p\n", client_pack->client_package->login_info, client_pack->client_package->channels);
    printf("\n\n\nDANS HANDLE CLIENT PRINT DE SEND_TYPE APRES SORTIE DE LOGIN ATTEMPT : %d\n\n\n", client_pack->client_package->send_type);
    if (login_status == 1)
    {
        printf("[INFO] dans handle client : Abandon de connexion");
        free(client_pack->client_package->client);
        free(client_pack->client_package);
        free(client_pack);
        pthread_exit(NULL);
        return 0;
    }

    if (!client_pack->client_package || !client_pack->client_package->client || !client_pack->client_package->login_info) {
        fprintf(stderr, "serialize_client_package: NULL field detected\n");
        if (!client_pack->client_package->client) {
            printf("c'est client_data qui est vide!");
        } else if (!client_pack->client_package->login_info) {
            printf("c'est login info qui est vide! ");
        }
    }

    printf("[handle_client before first_update_client_package] CHECKPOINT: channels %p\n", client_pack->client_package->channels);
    
    first_update_client_package(client_pack->client_package);
    printf("\n\n\nDANS HANDLE CLIENT PRINT DE SEND_TYPE APRES FIRST UPDATE CLIENT : %d\n\n\n", client_pack->client_package->send_type);
    // printf("[handle_client after first_update_client_package]CHECKPOINT: login_info ptr = %p\n channels %p\n", client_pack->client_package->login_info, client_pack->client_package->channels);
    if (client_pack->client_package->login_info == NULL) {
        fprintf(stderr, "login_info is NULL! Cannot access username.\n");
        pthread_exit(NULL);
    }

    printf("de retour dans handle_client : valeur user_id %d et user_name : %s\n",
        client_pack->client_package->login_info->user_id,
        client_pack->client_package->login_info->username);
    
    Client_data *client_data_copy = malloc(sizeof(Client_data)); // rajoute le mercredi soir
    if (!client_data_copy) {
        perror("malloc client_copy");
        printf("client_data_copy malloc failed");
        exit(EXIT_FAILURE);
    }

    Login_infos *login_copy = malloc(sizeof(Login_infos));
    if (!login_copy) {
        perror("malloc login_copy");
        exit(EXIT_FAILURE);
    }

    Client_package *client_copy = malloc(sizeof(Client_package));
    if (!client_copy) {
        perror("malloc client_copy");
        printf("client_copy malloc failed");
        exit(EXIT_FAILURE);
    }
    *login_copy = *(client_pack->client_package->login_info);
    *client_data_copy = *client;
    // *client_copy->client = *client; // fails !!!
    client_copy->client = client_data_copy;
    client_copy->login_info = login_copy;

    printf("sock du client : %lld\n", client_sock);

    pthread_mutex_lock(&state->lock);
    state->clients[state->client_count++] = client;
    pthread_mutex_unlock(&state->lock);

    printf("dans handle_client avant recover_messages\n");
    if (!client_pack->client_package || !client_pack->client_package->client || !client_pack->client_package->login_info) {
        fprintf(stderr, "serialize_client_package: NULL field detected\n");
        if (!client_pack->client_package->client) {
            printf("c'est client_data qui est vide!");
        } else if (!client_pack->client_package->login_info) {
            printf("c'est login info qui est vide! ");
        }
    }
    printf("\n\n\n ligne 359 PREMIER RECOVER MESSAGE\n\n\n");
    printf("[handle_client before recover_messages] CHECKPOINT: channels %p\n", client_pack->client_package->channels);
    recover_messages(client_pack);

    while (1)
    {
        // Message *client_message = malloc(sizeof(Message));
     
        // Client_package *client_package = malloc(sizeof(Client_package));
        
        // printf("reception apres premier recover_message dans la boucle while\n\n");
        // int bytes = recv(client_sock, (char *)client_package, sizeof(Client_package), 0);
        // if (bytes <= 0)
        // {
        //     printf("haha, le client a disparu\n");
        //     break;
        // }
        // client_pack->client_package = client_package;
        // strcpy(client_package->message_send.timestamp, get_str_timestamp());

        // printf("\n\n\n ligne 376 DEUXIEME RECOVER MESSAGE\n\n\n");
        recover_messages(client_pack);
        // broadcast_message(client_package); // Envoie d'une notification de nouveau message dans la db aux clients connectés
        // free(client_message);
    }

    printf("[INFO] Client %d déconnecté.\n", client->user_id);
    close(client_sock);

    pthread_mutex_lock(&state->lock);
    for (int i = 0; i < state->client_count; i++)
    {
        if (state->clients[i] == client)
        {
            free(state->clients[i]);
            for (int j = i; j < state->client_count - 1; j++)
            {
                state->clients[j] = state->clients[j + 1];
            }
            state->client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&state->lock);

    free(client);
    free(client_pack->client_package);
    free(client_copy);
    free(client_pack);
    pthread_exit(NULL);
}

void start_server()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    Server_state *state = malloc(sizeof(Server_state));
    state->client_count = 0;
    pthread_mutex_init(&state->lock, NULL);

    int server_sock;
    struct sockaddr_in server, client;
    int client_size = sizeof(client);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET)
    {
        printf("[ERROR] Erreur socket: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("[ERROR] Erreur bind: %d\n", WSAGetLastError());
        closesocket(server_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 3) == SOCKET_ERROR)
    {
        printf("[ERROR] Erreur listen: %d\n", WSAGetLastError());
        closesocket(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Serveur en écoute sur le port %d\n", PORT);

    SOCKET new_sock;
    printf("\nserver_sock avant accept() %lld\n", server_sock);
    while ((new_sock = accept(server_sock, (struct sockaddr *)&client, &client_size)))
    {
        printf("valeur de sock après accept() %lld", new_sock);
        Client_data *client_data = malloc(sizeof(Client_data));
        client_data->sock_pointer = new_sock;
        printf("\nvaleur de sock dans sock_pointer de client_data %lld\n", client_data->sock_pointer);
        // Channel_info *channels = malloc(sizeof(Channel_info));

        // Login_infos *login_info = malloc(sizeof(Login_infos));
        Client_package_for_backend *client_pack = malloc(sizeof(Client_package_for_backend));
        Client_package *client_package = malloc(sizeof(Client_package));
        // Client_package *client_package = malloc(sizeof(Client_package));
        memset(client_package, 0, sizeof(Client_package));

        Login_infos *login_info = malloc(sizeof(Login_infos));
        if (!login_info) {
            perror("malloc login_info");
            exit(EXIT_FAILURE);
        }

        // Channel_info *channel_info = malloc(sizeof(Channel_info));
        // if (!channel_info) {
        //     perror("malloc login_info");
        //     exit(EXIT_FAILURE);
        // }

        client_pack->client_package = client_package;
        client_pack->client_package->client = client_data;
        client_pack->client_package->login_info = login_info;
        printf("\nvaleur de sock dans sock_pointer de client_pack->client_package->client %lld\n",
            client_pack->client_package->client->sock_pointer);
        client_pack->server = state;
        // client_package->client->channels = &channels;
        // client_package->login_info = login_info;
        printf("start_server: login_info pointer = %p\n", login_info);
        printf("start_server: assigned to client_pack->client_package->login_info = %p\n", client_pack->client_package->login_info);

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, (void *)client_pack);
        pthread_detach(thread);
    }

    printf("Serveur stopped\n");
    WSACleanup();
}