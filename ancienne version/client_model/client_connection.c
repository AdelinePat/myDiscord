#include "../model/header/connection_sockets_test.h"

int __cdecl main(int argc, char const *argv[])
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    int InitClientWinsock;
    int ClientSocketAdrr;
    int ConnectResult;
    int iSendResult;
    
    int recvbuflen = DEFAULT_BUFLEN;

    const char *sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];

// Initialize Winsock
    InitClientWinsock = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (InitClientWinsock != 0) {
        printf("WSAStartup failed: %d\n", InitClientWinsock);
        return 1;
    }
    printf("\n\nWinsock successfully started client side ?\n");
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    
    

    // Resolve the server address and port
    // ClientSocketAdrr = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    ClientSocketAdrr = getaddrinfo("127.0.0.1", "8080", &hints, &result);
    if (ClientSocketAdrr != 0) {
        printf("getaddrinfo failed: %d\n", ClientSocketAdrr);
        WSACleanup();
        return 1;
    }
    printf("ClientSocketAddr value is ..... %d\n", ClientSocketAdrr);

    
    // Attempt to connect to the first address returned by
    // the call to getaddrinfo
    ptr=result;

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
        ptr->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    printf("Create client socket ? value %ld\n", ConnectSocket);

    // Connect to server.
    ConnectResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (ConnectResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }
    printf("Connection to server ? How does it know who the server is ??? value %ld\n", ConnectResult);

    // Should really try the next address returned by getaddrinfo
    // if the connect call failed
    // But for this simple example we just free the resources
    // returned by getaddrinfo and print an error message

    freeaddrinfo(result);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // #define DEFAULT_BUFLEN 512

    // Send an initial buffer
    iSendResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
    if (iSendResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iSendResult);

    // shutdown the connection for sending since no more data will be sent
    // the client can still use the ConnectSocket for receiving data
    iSendResult = shutdown(ConnectSocket, SD_SEND);
    if (iSendResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive data until the server closes the connection
    do {
        iSendResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iSendResult > 0)
            printf("Bytes received: %d\n", iSendResult);
        else if (iSendResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());
    } while (iSendResult > 0);

    // shutdown the send half of the connection since no more data will be sent
    iSendResult = shutdown(ConnectSocket, SD_SEND);
    if (iSendResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
        // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}