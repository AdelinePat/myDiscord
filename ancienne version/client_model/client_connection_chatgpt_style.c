#include "../model/header/connection_sockets_test.h"
// #pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL, hints;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_protocol = IPPROTO_TCP;  // TCP

    // Resolve the server address and port
    iResult = getaddrinfo("127.0.0.1", "8080", &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }
    printf("\n\nresult of getaddrinfo %d\n", iResult);

    // Create a socket for connecting to server
    ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Socket failed: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    printf("result of ConnectSocket using socket function %ld\n", ConnectSocket);

    // Connect to server
    iResult = connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("Connect failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        WSACleanup();
        return 1;
    }
    printf("result of iResult using connect function %ld\n", iResult);

    freeaddrinfo(result); // done with address structure

    printf("Connected to server successfully!\n");

    // Example of sending something (optional)
    const char* msg = "Hello from client!";
    send(ConnectSocket, msg, (int)strlen(msg), 0);

    // Clean up
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
