#ifndef DB_CHAT_H
#define DB_CHAT_H
#include "database_communication.h"
#include "../../controller/header/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_channel_list(Client_package *client_package);
PGresult* generate_channel_list_query(PGconn *conn, Client_package *client_package);
void get_full_chat_content(Client_package *client_package);
PGresult* generate_full_chat_content_query(PGconn *conn, Client_package *client_package);
int check_channel_access(PGconn *conn, Client_package *client_package);

#endif