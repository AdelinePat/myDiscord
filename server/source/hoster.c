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
        client_package->login_info = malloc(sizeof(Login_infos));
        // Login_infos *login_info = malloc(sizeof(Login_infos));
        int login_status = 0;
        // Client_package *client_package_copy = malloc(sizeof(Client_package));
        // Login_infos *login_info_copy = malloc(sizeof(Login_infos));
        // client_package_copy->login_info = login_info_copy;
        // int bytes = recv(client_package->client->sock_pointer, (char *)login_info, sizeof(Login_infos), 0);
        int len = 0;
        int check_len = recv(client_package->client->sock_pointer, (char *)&len, sizeof(int), 0);
        if (check_len == SOCKET_ERROR) {
            printf("[ERROR] Failed socket connexion from client to server\n");
        }

        if (check_len != sizeof(int))
        {
            printf("[ERROR] Failed to receive message length. Received %d bytes\n", check_len);
            break;
        }

        char * client_package_str = malloc(len + 1);

        printf("la longueur du json est de %d", (int)len);

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

        cJSON *root = cJSON_Parse(client_package_str);
        free(client_package_str);

        client_package->send_type = cJSON_GetObjectItem(root, "send_type")->valueint;

        cJSON *login = cJSON_GetObjectItem(root, "login_info");
        strcpy(client_package->login_info->username, cJSON_GetObjectItem(login, "username")->valuestring);
        strcpy(client_package->login_info->password, cJSON_GetObjectItem(login, "password")->valuestring);
        // client_package_copy = client_package;
        // printf("\n\n SEND_TYPE VALUE AFTER RECV : %d\n\n", client_package_copy->send_type);

        // client_package->login_info = client_package_copy->login_info;
        // client_package->send_type = client_package_copy->send_type;

        // printf("avant check dans copy : user << %s >> et password << %s >>",
        //     client_package_copy->login_info->username,
        //     client_package_copy->login_info->password);

        printf("Infos reçues : user : << %s >>, pass : << %s >>\n",
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
                { // condition de vérification des identifiants
                    // query ici pour l'id unique et le pseudo du client
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

        
        // bytes = "\0";
        // fonction_nimporte(login_info);      *
        // free(client_package_copy);  
        
    }
    
    return 1;
}

void recover_messages(Client_package_for_backend *package)
{
    get_full_chat_content(package->client_package);
    printf("sock du client : %lld\n", package->client_package->client->sock_pointer);

    int size_of_list = package->client_package->number_of_messages;
    
    printf("message_list size (number of messages) : %d", size_of_list);

    send(package->client_package->client->sock_pointer,
        (char *)&package->client_package->number_of_messages,
        sizeof(int),
        0);

    int validation;
    recv(package->client_package->client->sock_pointer, (char *)validation, sizeof(int), 0);

    printf("sending messages");
    printf("messages number : %d\n", package->client_package->number_of_messages);

    for (int i = 0; i < size_of_list; i++) {
        Message message_sent = package->client_package->messages_list[i];
        printf("[DEBUG] Envoi message %s : %s\n", message_sent.username, message_sent.message);

        int bytes = send(package->client_package->client->sock_pointer,
            (char *)&message_sent,
            sizeof(Message),
            0);

        if (bytes <= 0) {
            perror("[ERROR] send message failed\n");
            break;
        }
        recv(package->client_package->client->sock_pointer, (char *)validation, sizeof(int), 0);
    }

    printf("       messages sent\n");
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
    // Client_package *client_package = (Client_package *)arg;
    Client_package_for_backend *client_pack = (Client_package_for_backend *)arg;
    // Login_infos *login_info = malloc(sizeof(Login_infos));
    // client_package->login_info = login_info;
    // Client_package *client_package = malloc(sizeof(Client_package));
    // client_pack->client_package = client_package;

    // int login_status = login_attempts(client_package);
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

    // Client_package_for_backend *client_pack = malloc(sizeof(Client_package_for_backend));
    // client_pack->client_package = client_package;
    Server_state *state = client_pack->server;
    Client_data *client = client_pack->client_package->client;
    SOCKET client_sock = client->sock_pointer;

    int login_status = login_attempts(client_pack->client_package);
    if (login_status == 1)
    {
        printf("[INFO] dans handle client : Abandon de connexion");
        free(client_pack->client_package->client);
        free(client_pack->client_package);
        free(client_pack);
        pthread_exit(NULL);
        return 0;
    }

    first_update_client_package(client_pack->client_package);
    printf("de retour dans handle_client : valeur user_id %d et user_name : %s\n",
        client_pack->client_package->login_info->user_id,
        client_pack->client_package->login_info->username);
    // client->client_id = user_id;
    // strcpy(client->client_name, "user"); // Ajout des données client avant de les envoyer après connexion

    // Client_data *client_copy = malloc(sizeof(Client_data)); // On crée une copie de client pour ne pas créer de conflit dans la mémoire entre serveur et client
    Client_package *client_copy = malloc(sizeof(Client_package));
    *client_copy->client = *client;
    printf("sock du client : %lld\n", client_sock);
    send(client_sock, (char *)client_copy->client, sizeof(Client_package), 0);
    printf("[INFO] Client %d connecté.\n",
        client_pack->client_package->login_info->user_id);

    pthread_mutex_lock(&state->lock);
    state->clients[state->client_count++] = client;
    pthread_mutex_unlock(&state->lock);

    recover_messages(client_pack);

    while (1)
    {
        Message *client_message = malloc(sizeof(Message));
        // Client_package_for_backend *client_pack = malloc(sizeof(Client_package_for_backend));
        Client_package *client_package = malloc(sizeof(Client_package));
        
        // int bytes = recv(client_sock, (char *)client_message, sizeof(Message), 0);
        int bytes = recv(client_sock, (char *)client_package, sizeof(Client_package), 0);
        if (bytes <= 0)
        {
            printf("haha, le client a disparu\n");
            break;
        }
        client_pack->client_package = client_package;
        // switch client_package-
        strcpy(client_package->message_send.timestamp, get_str_timestamp());

        // Enregistrement du message dans la db
        recover_messages(client_pack);
        // broadcast_message(client_package); // Envoie d'une notification de nouveau message dans la db aux clients connectés
        free(client_message);
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
        client_pack->client_package = client_package;
        client_pack->client_package->client = client_data;
        printf("\nvaleur de sock dans sock_pointer de client_pack->client_package->client %lld\n",
            client_pack->client_package->client->sock_pointer);
        client_pack->server = state;
        // client_package->client->channels = &channels;
        // client_package->login_info = login_info;

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, (void *)client_pack);
        pthread_detach(thread);
    }

    printf("Serveur stopped\n");
    WSACleanup();
}