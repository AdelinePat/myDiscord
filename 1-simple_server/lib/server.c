#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

void *handle_client(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char buffer[1024];
    int read_size;

    printf("Client connecté.\n");

    while ((read_size = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[read_size] = '\0';  // On termine proprement la chaîne
        printf("Message reçu : %s", buffer); // pas besoin de \n, fgets l'inclut déjà

        send(sock, buffer, strlen(buffer), 0);
    }

    printf("Client déconnecté.\n");
    close(sock);
    free(socket_desc);
    pthread_exit(NULL);
}

int main() {
    int server_fd, client_sock, *new_sock;
    struct sockaddr_in server, client;
    socklen_t c = sizeof(client);

    server_fd = socket(AF_INET, SOCK_STREAM, 0); 
        //AF_INET stands for IPv4, SOCK_STREAM is the type of socket made for TCP 
        // connection, and 0 leaves choice to the machine for the protocol (TCP)
    if (server_fd == -1) {
        perror("Erreur socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET; // famille d'adresse IPv4
    server.sin_addr.s_addr = INADDR_ANY; // tpye d'adresses acceptées only locales, but any
    server.sin_port = htons(PORT); // on translate le port en format "réseau" (host to network short)

    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Erreur bind");
        exit(EXIT_FAILURE);
    }

    listen(server_fd, 3);
    printf("Serveur en écoute sur le port %d\n", PORT);

    while ((client_sock = accept(server_fd, (struct sockaddr *)&client, &c))) {
        new_sock = malloc(sizeof(int));
        *new_sock = client_sock;
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, (void*)new_sock) < 0) {
            perror("Erreur thread");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
