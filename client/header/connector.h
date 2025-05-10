#ifndef CONNECTOR_H
#define CONNECTOR_H
#include "./client_front.h"
// #include <winsock2.h>
// #include "../../controller/header/utils.h"

// void client_start();
SOCKET client_start();
// int login_attempts(SOCKET sock);
int login_attempts(Client_package_for_frontend *login_pack);
int connect_to_server(Client_package_for_frontend *login_pack);
int register_attempts(Client_package_for_frontend *login_pack);
void *receive_messages(void *arg);
// void recover_messages(SOCKET sock, GtkWidget *chat_display);
void recover_messages(Client_package_for_frontend *login_pack);
void receive_client_data(Client_package_for_frontend *login_pack);
void broadcast_notifications_receiver_start(Client_package_for_frontend *login_pack);
void send_message(Client_package *client_package, char text[1024]);
// void fill_in_structures(Client_package_for_frontend *login_pack);
// SendType register_attempts(Login_package_for_front *login_pack);

#endif