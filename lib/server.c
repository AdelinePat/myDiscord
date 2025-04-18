#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>         // close()
#include <arpa/inet.h>      // inet_addr, htons etc.
#include <pthread.h>        // threads

#define PORT 4242

void *handle_client(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char buffer[1024];
    int read_size;

    // Réception du message du client
    while ((read_size = recv(sock, buffer, 1024, 0)) > 0) {
        buffer[read_size] = '\0';
        printf("Client dit : %s\n", buffer);
    }

    if (read_size == 0) {
        puts("Client déconnecté");
    } else if (read_size == -1) {
        perror("Erreur recv");
    }

    close(sock);
    free(socket_desc);
    pthread_exit(NULL);
}

int main() {
    int server_fd, client_sock, *new_sock;
    struct sockaddr_in server, client;
    socklen_t c = sizeof(struct sockaddr_in);

    // Création de la socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Échec socket");
        return 1;
    }
    puts("Socket créée");

    // Configuration de l'adresse
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Liaison
    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Échec bind");
        return 1;
    }
    puts("Bind réussi");

    // Écoute
    listen(server_fd, 3);
    puts("En attente de connexions...");

    // Boucle d'acceptation des connexions
    while ((client_sock = accept(server_fd, (struct sockaddr *)&client, &c))) {
        puts("Connexion acceptée");

        pthread_t client_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if (pthread_create(&client_thread, NULL, handle_client, (void*)new_sock) < 0) {
            perror("Impossible de créer le thread");
            return 1;
        }

        puts("Thread assigné au client");
    }

    if (client_sock < 0) {
        perror("accept échoué");
        return 1;
    }

    return 0;
}