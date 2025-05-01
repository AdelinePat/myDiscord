#ifndef DB_CONNEXION_H
#define DB_CONNEXION_H
#include "database_communication.h"
#include "../../controller/header/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_user_pass_match(Login_infos* login_info);
void first_update_client_package(Client_package *client_package);
void get_user_data(Client_package* client_package);


#endif