#include "../header/connection_sockets_test.h"

int main ()
{
    WSADATA wsaData; // whatever object it is
    int WinInitResult;
    int AddrInitResult;
    int BindResult;
    /* Windows Socket API, also known as Winsock,
    is a type of application programming interface (API)
    used to communicate between Windows network software
    and network services. It’s primarily based on
    Transmission Control Protocol/Internet Protocol (TCP/IP),
    and derives its roots from the Berkeley Unix sockets interface.
    
    Winsock allows Windows programs and applications to connect to the internet through TCP/IP.
    Winsock exists as a data link layer, and is also known as winsock.dll in our computers.
    
    Winsock serves as a translator for basic network services, such as send () or receive () requests.*/

    // Initialize Winsock
    WinInitResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (WinInitResult != 0) {
        printf("WSAStartup failed: %d\n", WinInitResult);
        return 1;
    }
    printf("\n\nYay, winsock initialized successfully, result value : %d\n", WinInitResult);
    // End Initializing Winsock

    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    // hints.ai_addr = inet_addr("127.0.0.1");
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    /* Resolve the local address and port to be used by the server
     AddrInitResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    The getaddrinfo function is used to determine the values in the sockaddr structure.*/ 
    AddrInitResult = getaddrinfo("127.0.0.1", "8080", &hints, &result); 
    if (AddrInitResult != 0) {
        printf("getaddrinfo failed: %d\n", AddrInitResult);
        WSACleanup();
        return 1;
    }
    printf("Getaddrinfo worked and here its content, should be 0 lol : %ld\n", AddrInitResult);

    SOCKET ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    printf("Listensocket is created successfully, I guess. Who knows what kind of shit I'm doing anyway , value = %ld\n", ListenSocket);

    // Setup the TCP listening socket
    BindResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (BindResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    /* Once the bind function is called, the address information returned
    by the getaddrinfo function is no longer needed. The freeaddrinfo
    function is called to free the memory allocated by the getaddrinfo
    function for this address information */
    freeaddrinfo(result); 
    printf("Yay, the socket is bound ?! Hopefully... %ld\n", BindResult);


    /* 
    To listen on a socket
    Call the listen function, passing as parameters the
    created socket and a value for the backlog, maximum length
    of the queue of pending connections to accept. In this example,
    the backlog parameter was set to SOMAXCONN. This value is a special
    constant that instructs the Winsock provider for this socket to
    allow a maximum reasonable number of pending connections in the queue.
    Check the return value for general errors.
    */
    if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "Listen failed with error: %ld\n", WSAGetLastError() );
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    printf("Do I listen to the ListenSocket ? I probably am.\n");

    // Creating socket "object" for clientsocket
    SOCKET ClientSocket;

    // accept function : first server socket (in , the one that listens) and then socket addr for client (out), then addr len for in and out ? How the hell does it work ?
    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    printf("Did I actually accept the clientsocket connection ?!\nClientSocket value : %ld\n", ClientSocket);

    // No longer need server socket
    

    char recvbuf[DEFAULT_BUFLEN];
    int iResult, iSendResult;
    int recvbuflen = DEFAULT_BUFLEN;

    // Receive until the peer shuts down the connection
    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            // Echo the buffer back to the sender
            iSendResult = send(ClientSocket, recvbuf, iResult, 0);
            printf("iSendResult before conditions: %d\n", iSendResult);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        } else if (iResult == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
    } while (iResult > 0);

    
    // shutdown the send half of the connection since no more data will be sent
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    closesocket(ListenSocket);
    printf("Server socket closed 'cause it has enough of this shit\n");
    
    WSACleanup();
    return 0;

}