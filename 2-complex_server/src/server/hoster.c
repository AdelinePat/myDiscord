#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "utils.h"

#define PORT 8080

int login_attempts(Client_package *client_package) {
    while (1) {
        Login_infos login_info;
        int login_status = 0;
        int bytes = recv(client_package->client->sock_pointer, (char *)&login_info, sizeof(Login_infos), 0);
        if (bytes == SOCKET_ERROR) {
            printf("[ERROR] Erreur lors de la réception de Login_infos, code %d\n", WSAGetLastError());
            break;
        }
        if (bytes != sizeof(Login_infos)) {
            printf("[ERROR] Taille des données reçues incorrecte. Attendu %zu, reçu %d\n", sizeof(Login_infos), bytes);
            break;
        }
        printf("Infos reçues\n");

        if (strcmp(login_info.username, "user") == 0 && strcmp(login_info.password, "pass") == 0) { // condition de vérification des identifiants
            // query ici pour l'id unique et le pseudo du client
            login_status = 1;
            send(client_package->client->sock_pointer, (char *)&login_status, sizeof(login_status), 0);
            printf("Connexion réussie\n");
            return client_package->server->client_count; // remplacer le return avec l'id du client dans la db
        } else {
            printf("Informations de connexion erronées.\n");
            send(client_package->client->sock_pointer, (char *)&login_status, sizeof(login_status), 0);
        }
    }
    return -1;
}

void broadcast_message(Server_state *state, Message *client_message) {
    char formatted[1200];
    char time_str[16];

    strftime(time_str, sizeof(time_str), "[%H:%M:%S]", &client_message->timestamp);
    snprintf(formatted, sizeof(formatted), "%s [Client %d] %s", time_str, client_message->client_id, client_message->message);

    pthread_mutex_lock(&state->lock);
    for (int i = 0; i < state->client_count; i++) {
        send(state->clients[i]->sock_pointer, formatted, strlen(formatted), 0);
    }
    pthread_mutex_unlock(&state->lock);
}

void *handle_client(void *arg) {
    Client_package *client_package = (Client_package *)arg;

    int user_id = login_attempts(client_package);

    if (user_id < 0) {
        printf("[INFO] Abandon de connexion");
        free(arg);
        pthread_exit(NULL);
        return 0;
    }

    Server_state *state = client_package->server;
    Client_data *client = client_package->client;
    SOCKET sock = client->sock_pointer;

    client->client_id = user_id;
    strcpy(client->client_name, "user"); // Ajout des données client avant de les envoyer après connexion

    Client_data *client_copy = malloc(sizeof(Client_data)); // On crée une copie de client pour ne pas créer de conflit dans la mémoire entre serveur et client

    *client_copy = *client;
    send(sock, (char *)client_copy, sizeof(Client_data), 0);
    printf("[INFO] Client %d connecté.\n", client_package->client->client_id);

    pthread_mutex_lock(&state->lock);
    state->clients[state->client_count++] = client;
    pthread_mutex_unlock(&state->lock);

    while (1) {
        Message *client_message = malloc(sizeof(Message));
        printf("hihi1 sock : %d\n", sock);
        int bytes = recv(sock, (char *)client_message, sizeof(Message), 0);
        if (bytes <= 0) {
            printf("haha\n");
            break;
        }
        printf("hihi2\n");
        client_message->timestamp = get_timestamp();

        // Enregistrement du message dans la db
        printf("hihi3\n");
        broadcast_message(state, client_message); // Envoie d'une notification de nouveau message dans la db aux clients connectés
        printf("hihi4\n");
        free(client_message);
    }

    printf("[INFO] Client %d déconnecté.\n", client->client_id);
    close(sock);

    pthread_mutex_lock(&state->lock);
    for (int i = 0; i < state->client_count; i++) {
        if (state->clients[i] == client) {
            free(state->clients[i]);
            for (int j = i; j < state->client_count - 1; j++) {
                state->clients[j] = state->clients[j + 1];
            }
            state->client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&state->lock);

    free(arg);
    pthread_exit(NULL);
}

void start_server() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    
    Server_state *state = malloc(sizeof(Server_state));
    state->client_count = 0;
    pthread_mutex_init(&state->lock, NULL);

    int server_fd;
    struct sockaddr_in server, client;
    int client_size = sizeof(client);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        printf("[ERROR] Erreur socket: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("[ERROR] Erreur bind: %d\n", WSAGetLastError());
        closesocket(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) == SOCKET_ERROR) {
        printf("[ERROR] Erreur listen: %d\n", WSAGetLastError());
        closesocket(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Serveur en écoute sur le port %d\n", PORT);

    SOCKET new_sock;
    while ((new_sock = accept(server_fd, (struct sockaddr *)&client, &client_size))) {
        Client_data *client_data = malloc(sizeof(Client_data));
        client_data->sock_pointer = new_sock;

        Client_package *client_package = malloc(sizeof(Client_package));
        client_package->client = client_data;
        client_package->server = state;
        
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, (void *)client_package);
        pthread_detach(thread);
    }

    printf("Serveur stopped\n");
    WSACleanup();
}