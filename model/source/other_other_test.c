// test_winsock.c
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

int main() {
    WSADATA wsa;
    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        return 1;
    }
    printf("Initialized.\n");
    WSACleanup();
    return 0;
}
