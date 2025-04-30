#ifndef CONNEXION_H
#define CONNEXION_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libpq-fe.h>
#include "../../controller/header/utils.h"
#define ENV_PATH "C:\\Users\\Adeline\\Documents\\Projetdev\\myDiscord\\.env"
#define MAX_STRING_SIZE 100

// void load_variable(FILE* file, char* password, char* user_name);
// int database_connexion_info(char* password, char* user_name);

PGconn * database_connexion();
void load_variable(FILE* file, char* database_password, char* database_user_name);

#endif
