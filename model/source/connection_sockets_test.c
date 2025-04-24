
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include <netdb.h>
#include <netinet/in.h>

// CREATE SOCKET DESCRIPTOR sockfd = Socket File Descriptor : int sockfd = socket(domain, type, protocol);
int sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);

// CREATE A SOCKET OPTION TO MANIPULATE FILE DESCRIPTOR
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

// backlock = The backlog, defines the maximum length to which the queue of pending connections for sockfd may grow. If a connection request arrives when the queue is full, the client may receive an error with an indication of ECONNREFUSED.
int listen(int sockfd, int backlog);


