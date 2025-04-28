#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <process.h> // pour _beginthread
#include <string.h>

#pragma comment(lib, "Ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 512

void recv_thread(void *socketPtr)
{
    SOCKET s = *(SOCKET *)socketPtr;
    char buffer[BUFFER_SIZE];
    int iResult;

    while (1)
    {
        iResult = recv(s, buffer, BUFFER_SIZE - 1, 0);
        if (iResult > 0)
        {
            buffer[iResult] = '\0';
            printf("\n[Message reçu] %s> ", buffer);
            fflush(stdout);
        }
        else
        {
            printf("\n[Serveur déconnecté]\n");
            break;
        }
    }
    _endthread();
}

int main()
{
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed.\n");
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET)
    {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(clientSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("Connection failed.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    printf("Connected to server!: Shadows\n");

    // Lancer le thread de réception
    _beginthread(recv_thread, 0, &clientSocket);

    // Boucle d'envoi
    while (1)
    {
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(clientSocket, buffer, (int)strlen(buffer), 0);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
