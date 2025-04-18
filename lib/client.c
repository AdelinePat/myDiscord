#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <winsock2.h>

#define PORT 4242

int main() {
    int sock;
    struct sockaddr_in server;
    char message[1024];

    // Création de la socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Échec de la socket");
        return 1;
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Connexion au serveur
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connexion échouée");
        return 1;
    }

    puts("Connecté au serveur");

    // Envoyer des messages
    while (1) {
        printf("Moi : ");
        fgets(message, 1024, stdin);
        send(sock, message, strlen(message), 0);
    }

    close(sock);
    return 0;
}
