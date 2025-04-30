#include "../header/database_communication.h"

void load_variable(FILE* file, char* database_password, char* database_user_name)
{
    file = fopen(ENV_PATH, "r+");
    if (file != NULL)
    {
        strncpy(database_password, fgets(database_password, MAX_STRING_SIZE, file), MAX_STRING_SIZE);
        strncpy(database_user_name, fgets(database_user_name, MAX_STRING_SIZE, file), MAX_STRING_SIZE);
        printf("DANS LA FONCTION LOAD VARIABLE \n user_name : %s\npassword : %s\n\n", database_user_name, database_password);
        // On peut lire et écrire dans le fichier
        fclose(file);
    }
    else
    {
        // On affiche un message d'erreur si on veut
        printf("Impossible d'ouvrir le fichier test.txt");
    }
}

int database_connexion_info(char* db_password, char* db_user_name)
{
    printf("AVANT LOAD VARIALBLE :\nuser_name : %s\npassword : %s\n\n", db_user_name, db_password);

    FILE* file = NULL;
    load_variable(file, db_password, db_user_name);
    printf("APRES LOAD VARIABLE\nuser_name : %s\npassword : %s", db_user_name, db_password);
    
    // Count how many caracter for the string
    int final_size_conninfo = snprintf(NULL, 0,
        "dbname = whispr user=%s password=%s host=localhost port=5432", db_user_name, db_password) + 1;

    return final_size_conninfo;
}

PGconn * database_connexion()
{  
    char db_password[MAX_STRING_SIZE] = "";
    char db_user_name[MAX_STRING_SIZE] = "";
    
    int final_size_conninfo = database_connexion_info(db_password, db_user_name);

    printf("\n\nfinal size %d", final_size_conninfo);
    const char conninfo[final_size_conninfo];

    snprintf(conninfo, final_size_conninfo, "dbname = whispr user=%s password=%s host=localhost port=5432", db_user_name, db_password);
    printf("\n\nconninfo c'est : %s", conninfo);

    PGconn *conn;   
    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK)
    {
        printf("Erreur lors de la connexion");
        PQfinish(conn);
        exit(1);
    }

    printf("Connexion établie,\nport utilisé %s\nhostname %s\nDBname : %s", PQport(conn), PQhost(conn), PQdb(conn));
    return conn;
}

int connection_query(Login_infos login_info) {
    PGconn *conn = database_connexion();
    PGresult *res = NULL;
    int nFields;

    int final_size_query = snprintf(NULL, 0,
        "SELECT COUNT(user_name) FROM users\n\
    WHERE user_name = '%s' AND password = '%s'", login_info.username, login_info.password) +1;

    char *query = malloc(final_size_query);
    if (query == NULL) {
        fprintf(stderr, "Memory allocation failed for query.\n");
        PQfinish(conn);
        exit(1);
    }

    snprintf(query, final_size_query,
        "SELECT COUNT(user_name) FROM users\n\
    WHERE user_name = '%s' AND password = '%s'", login_info.username, login_info.password);

    // multiple line query add \n\ at the end of every line OR quote at the begging of every line

    res = PQexec(conn, query);
    free(query);

    ExecStatusType resStatus = PQresultStatus(res);
    
    if (resStatus != PGRES_TUPLES_OK)
    {
        printf("il y a eu une erreur dans la requête");
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }

    int rows = PQntuples(res);
    int cols = PQnfields(res);

    printf("\n");
    // print column name i don t care about it
    // for (int i = 0; i < cols; i++)
    // {
    //     printf("%s\t", PQfname(res, i));
    // }
    // printf("\n");
    int query_result = 0;
    printf("value du result de la query mdr : %d", query_result);

    char *result_str = PQgetvalue(res, 0, 0);
    query_result = atoi(result_str);  // Convert string to int

    // for (int y = 0; y < rows; y++)
    // {
    //     for (int j = 0; j < cols ; j++)
    //     {
    //         query_result = (int)PQgetvalue(res, y, j);
    //         printf("%s\t", PQgetvalue(res, y, j));
    //     }
    //     printf("\n");
    // }

    PQclear(res);

    PQfinish(conn);
    printf("\n\nBye bye\n\n");
    
    return query_result;
}


//     nothing to see;

//     // strcmp(login_info.username, "Anya") == 0 && strcmp(login_info.password, "pass")

// }
    
    
    // char *query = "SELECT user_name FROM users";
    // char *query = "SELECT c.channel_title, COALESCE(u.user_name, 'Anonymous user'), m.date_time, m.content\n\
    //                 FROM messages AS m\n\
    //                 JOIN channels AS c ON c.channel_id = m.channel_id\n\
    //                 LEFT JOIN users AS u ON u.user_id = m.user_id\n\
    //                 WHERE c.channel_status = 'public' AND c.channel_id = (\n\
    //                     SELECT channel_id FROM channels\n\
    //                     WHERE channel_title = 'General_chat'\n\
    //                 )\n\
    //                 ORDER BY date_time";
    
//     char *query = "SELECT c.channel_title, COALESCE(u.user_name, 'Anonymous user'), m.date_time, m.content "
//         "FROM messages AS m "
//         "JOIN channels AS c ON c.channel_id = m.channel_id "
//         "LEFT JOIN users AS u ON u.user_id = m.user_id "
//         "WHERE c.channel_id IN ( "
//             "SELECT channel_id FROM channels "
//             "WHERE channel_status = 'private' "
//         ") AND u.user_id IN ( "
//             "SELECT user_id "
//             "FROM channels_access "
//         " WHERE role_title != 'ban' AND channel_id = 3)";

//     // multiple line query add \n\ at the end of every line OR quote at the begging of every line

//     res = PQexec(conn, query);

//     ExecStatusType resStatus = PQresultStatus(res);
    
//     if (resStatus != PGRES_TUPLES_OK)
//     {
//         printf("il y a eu une erreur dans la requête");
//         PQclear(res);
//         PQfinish(conn);
//         exit(1);
//     }

//     int rows = PQntuples(res);
//     int cols = PQnfields(res);

//     printf("\n");
//     // print column name i don t care about it
//     for (int i = 0; i < cols; i++)
//     {
//         printf("%s\t", PQfname(res, i));
//     }
//     printf("\n");

//     for (int y = 0; y < rows; y++)
//     {
//         for (int j = 0; j < cols ; j++)
//         {
//             printf("%s\t", PQgetvalue(res, y, j));
//         }
//         printf("\n");
//     }

//     PQclear(res);

//     PQfinish(conn);
//     printf("\n\nBye bye\n\n");
//     return 0;
// }
