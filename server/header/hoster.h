#ifndef HOSTER_H
#define HOSTER_H

#include "../../controller/header/utils.h"

void start_server();
void *handle_client(void *arg);
int login_attempts(Client_package *client_package);
void broadcast_message(Client_package *client_package);
// int login_attempts(Client_package_for_backend *package);
void recover_messages(Client_package_for_backend *package);

#endif