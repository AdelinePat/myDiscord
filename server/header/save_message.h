#ifndef DB_SAVE_MESSAGE_H
#define DB_SAVE_MESSAGE_H
#include "database_communication.h"
#include "../../controller/header/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/database_communication.h"
#include "../header/db_connection.h"

PGresult * create_new_message_query(PGconn *conn, Client_package *client_package);
void create_new_message_db(Client_package* client_package);

#endif