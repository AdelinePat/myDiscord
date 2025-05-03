#ifndef CONNECTOR_H
#define CONNECTOR_H
#include "./client_front.h"
#include "../header/login_window.h"

// #include <winsock2.h>
// #include "../../controller/header/utils.h"

// void client_start();
SOCKET client_start();
// int login_attempts(SOCKET sock);
int login_attempts(Login_package_for_front *login_pack);
int register_attempts(Login_package_for_front *login_pack);
void *receive_messages(void *arg);
void receive_client_data(Login_package_for_front *login_pack);
void broadcast_notifications_receiver_start(Login_package_for_front *login_pack);
void send_message(Client_data *client, char text[1024]);
// SendType register_attempts(Login_package_for_front *login_pack);
int fetch_user_id_from_db(const char *username, const char *password);

#endif