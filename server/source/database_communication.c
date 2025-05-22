#include "../header/database_communication.h"

void load_variable(FILE* file, char* database_password, char* database_user_name)
{
    file = fopen(ENV_PATH, "r+");
    if (file != NULL)
    {
        strncpy(database_password, fgets(database_password, MAX_STRING_SIZE, file), MAX_STRING_SIZE);
        strncpy(database_user_name, fgets(database_user_name, MAX_STRING_SIZE, file), MAX_STRING_SIZE);
        fclose(file);
    }
    else
    {
        printf("Impossible d'ouvrir le fichier .env");
    }
}

int database_connexion_info(char* db_password, char* db_user_name)
{
    FILE* file = NULL;
    load_variable(file, db_password, db_user_name);
    
    int final_size_conninfo = snprintf(NULL, 0,
        "dbname = whispr user=%s password=%s host=localhost port=5432", db_user_name, db_password) + 1;

    return final_size_conninfo;
}

PGconn * database_connexion()
{  
    char db_password[MAX_STRING_SIZE] = "";
    char db_user_name[MAX_STRING_SIZE] = "";
    
    int final_size_conninfo = database_connexion_info(db_password, db_user_name);

    const char conninfo[final_size_conninfo];

    snprintf(conninfo, final_size_conninfo, "dbname = whispr user=%s password=%s host=localhost port=5432", db_user_name, db_password);
    PGconn *conn;   
    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK)
    {
        printf("Erreur lors de la connexion");
        PQfinish(conn);
        exit(1);
    }
    printf("Connexion avec la DB établie\n");
    return conn;
}