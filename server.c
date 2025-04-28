#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 512
#define MAX_CLIENTS FD_SETSIZE // Limite imposée par select()

int main()
{
    WSADATA wsaData;
    SOCKET serverSocket, clientSockets[MAX_CLIENTS];
    struct sockaddr_in serverAddr, clientAddr;
    int addrlen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];
    int iResult;

    // Init Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed.\n");
        return 1;
    }

    // Créer socket serveur
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    // Config adresse serveur
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(DEFAULT_PORT);

    // Bind
    if (bind(serverSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("Bind failed.\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("Listen failed.\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d...\n", DEFAULT_PORT);

    // Initialiser tableau de clients
    for (int i = 0; i < MAX_CLIENTS; i++)
        clientSockets[i] = 0;

    fd_set readfds;

    while (1)
    {
        // Réinitialiser le set
        FD_ZERO(&readfds);

        // Ajouter le socket serveur
        FD_SET(serverSocket, &readfds);
        SOCKET max_sd = serverSocket;

        // Ajouter les sockets clients
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            SOCKET s = clientSockets[i];
            if (s > 0)
                FD_SET(s, &readfds);
            if (s > max_sd)
                max_sd = s;
        }

        // Attendre une activité sur un des sockets
        int activity = select(0, &readfds, NULL, NULL, NULL);
        if (activity < 0)
        {
            printf("select error\n");
            break;
        }

        // Nouveau client ?
        if (FD_ISSET(serverSocket, &readfds))
        {
            SOCKET new_socket = accept(serverSocket, (SOCKADDR *)&clientAddr, &addrlen);
            if (new_socket == INVALID_SOCKET)
            {
                printf("accept failed.\n");
                continue;
            }

            printf("New client connected: socket %d\n", (int)new_socket);

            // Ajouter au tableau
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                if (clientSockets[i] == 0)
                {
                    clientSockets[i] = new_socket;
                    break;
                }
            }
        }

        // Clients existants
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            SOCKET s = clientSockets[i];
            if (FD_ISSET(s, &readfds))
            {
                iResult = recv(s, buffer, BUFFER_SIZE, 0);
                if (iResult <= 0)
                {
                    // Déconnexion
                    printf("Client disconnected: socket %d\n", (int)s);
                    closesocket(s);
                    clientSockets[i] = 0;
                }
                else
                {
                    buffer[iResult] = '\0';
                    printf("Message from client %d: %s\n", (int)s, buffer);

                    // Envoyer à tous les autres
                    for (int j = 0; j < MAX_CLIENTS; j++)
                    {
                        if (clientSockets[j] != 0 && clientSockets[j] != s)
                        {
                            send(clientSockets[j], buffer, iResult, 0);
                        }
                    }
                }
            }
        }
    }

    // Nettoyage (théorique ici, car boucle infinie)
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}