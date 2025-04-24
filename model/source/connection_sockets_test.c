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
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    AddrInitResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
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

    
    
    WSACleanup();
    return 0;

}