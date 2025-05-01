#include "../header/database_communication.h"
#include "../header/db_connection.h"
#include "../header/database_communication.h"
#include "../header/db_chat_content.h"

int check_user_pass_match(Login_infos* login_info) {
    PGconn *conn = database_connexion();
    PGresult *res = NULL;
    int nFields;

    int final_size_query = snprintf(NULL, 0,
        "SELECT COUNT(user_name) FROM users\n\
    WHERE (user_name = '%s' OR email = '%s') AND password = '%s'", login_info->username, login_info->username, login_info->password) +1;

    char *query = malloc(final_size_query);
    if (query == NULL) {
        fprintf(stderr, "Memory allocation failed for query.\n");
        PQfinish(conn);
        exit(1);
    }

    snprintf(query, final_size_query,
        "SELECT COUNT(user_name) FROM users\n\
    WHERE (user_name = '%s' OR email = '%s') AND password = '%s'", login_info->username, login_info->username, login_info->password);

    // multiple line query add \n\ at the end of every line OR quote at the begging of every line

    res = PQexec(conn, query);
    free(query);

    ExecStatusType resStatus = PQresultStatus(res);
    
    if (resStatus != PGRES_TUPLES_OK)
    {
        printf("il y a eu une erreur dans la requête qui compte le nombre d'utilisateur qui corresponde au username et password transmis par Gtk");
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }

    int rows = PQntuples(res);
    int cols = PQnfields(res);

    printf("\n");

    int query_result = 0;
    printf("value du result de la query mdr : %d", query_result);

    if (rows > 0 && cols > 0)
    {
        char *result_str = PQgetvalue(res, 0, 0);
        query_result = atoi(result_str);  // Convert string to int
    }
    else {
        printf("erreur dans la requête, aucune colonne ou ligne trouvée ?");
    }
    

    PQclear(res);

    PQfinish(conn);
    printf("\n\nBye bye\n\n");
    
    return query_result;
}

void first_update_client_package(Client_package *client_package) {
    client_package->login_info->current_channel = 1;
    get_user_data(client_package);
    get_channel_list(client_package);
    get_full_chat_content(client_package);
    // client_package->login_info->user_id = un int; // => resultat requête  
    // strcpy(client_package->login_info->username, resultatrequete);
    // strcpy(client_package->login_info->password, "");
    // strcpy(client_package->login_info->confirm_password, "");
    // strcpy(client_package->login_info->email, "");
}

void get_user_data(Client_package* client_package) {
    PGconn *conn = database_connexion();
    PGresult *res = NULL;
    int nFields;

    int final_size_query = snprintf(NULL, 0,
        "SELECT * FROM users\n\
    WHERE user_name = '%s' AND password = '%s'", client_package->login_info->username, client_package->login_info->password) +1;

    char *query = malloc(final_size_query);
    if (query == NULL) {
        fprintf(stderr, "Memory allocation failed for query.\n");
        PQfinish(conn);
        exit(1);
    }

    printf("user_name before query %s\n", client_package->login_info->username);
    printf("password before query %s\n", client_package->login_info->password);

    snprintf(query, final_size_query,
        "SELECT * FROM users\n\
    WHERE user_name = '%s' AND password = '%s'", client_package->login_info->username, client_package->login_info->password);
    
    res = PQexec(conn, query);
    free(query);

    ExecStatusType resStatus = PQresultStatus(res);
    
    if (resStatus != PGRES_TUPLES_OK)
    {
        printf("il y a eu une erreur dans la requête de récupération des données de l'utilisateur qui s'est connecté");
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }

    int rows = PQntuples(res);
    int cols = PQnfields(res);

    printf("\n");

    // char* query_result = 0;
    // printf("value du result de la query mdr : %d", query_result);

    if (rows > 0 && cols > 0)
    {
        char *result_user_id = PQgetvalue(res, 0, 0);
        char *result_user_name = PQgetvalue(res, 0, 1);
        int user_id = atoi(result_user_id);  // Convert string to int
        client_package->login_info->user_id = user_id;
        client_package->client->client_id = user_id;
        
        strcpy(client_package->login_info->username, result_user_name);
        strcpy(client_package->client->client_name, result_user_name);
        // printf("print du login info après la requête : id %d, name %s\n", client_package->login_info->user_id, client_package->login_info->username);
    }
    else {
        printf("erreur dans la requête, aucune colonne ou ligne trouvée ?\n");
    }
    

    PQclear(res);

    PQfinish(conn);
    printf("\n\nBye bye\n\n");
}