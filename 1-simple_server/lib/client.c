#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server;
    char message[1024];
    char response[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Erreur socket");
        exit(EXIT_FAILURE);
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Erreur connect");
        exit(EXIT_FAILURE);
    }

    printf("Connecté au serveur. Tape un message :\n");

    while (1) {
        printf("> ");
        if (fgets(message, sizeof(message), stdin) == NULL) {
            break; // fin de l’entrée
        }

        if (send(sock, message, strlen(message), 0) < 0) {
            perror("Erreur send");
            break;
        }

        int recv_size = recv(sock, response, sizeof(response) - 1, 0);
        if (recv_size > 0) {
            response[recv_size] = '\0';
            printf("CLIENT : %s", response);
        }
    }

    close(sock);
    return 0;
}
