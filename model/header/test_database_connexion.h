#ifndef CONNEXION_H
#define CONNEXION_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libpq-fe.h>
#define ENV_PATH "C:\\Users\\Adeline\\Documents\\Projetdev\\myDiscord\\.env"
#define MAX_STRING_SIZE 100

void load_variable(FILE* file, char* password, char* user_name);
int database_connexion_info(char* password, char* user_name);

#endif
