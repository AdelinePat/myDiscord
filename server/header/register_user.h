#ifndef DB_REGISTER_H
#define DB_REGISTER_H
#include "database_communication.h"
#include "../../controller/header/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/database_communication.h"
#include "../header/db_connection.h"

int does_user_exists(Login_infos* login_info);
PGresult* create_new_user_query(PGconn *conn, Client_package *client_package);
void create_new_user_db(Client_package* client_package);

#endif