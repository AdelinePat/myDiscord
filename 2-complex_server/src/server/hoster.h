#ifndef HOSTER_H
#define HOSTER_H

#include "utils.h"

void start_server();
void *handle_client(void *arg);
int login_attempts(Client_package *client_package);
void broadcast_message(Server_state *state, Message *client_message);

#endif