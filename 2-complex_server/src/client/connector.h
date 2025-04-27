#ifndef CONNECTOR_H
#define CONNECTOR_H
#include <winsock2.h>
#include "utils.h"

void client_start();
int login_attempts(SOCKET sock);
void *receive_messages(void *arg);
void send_message(Client_data *client, char text[1024]);

#endif