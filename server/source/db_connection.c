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
    WHERE (user_name = '%s' OR email = '%s') AND password = '%s'",
    login_info->username,
    login_info->email,
    login_info->password) +1;

    char *query = malloc(final_size_query);
    if (query == NULL) {
        fprintf(stderr, "Memory allocation failed for query.\n");
        PQfinish(conn);
        exit(1);
    }

    snprintf(query, final_size_query,
        "SELECT COUNT(user_name) FROM users\n\
    WHERE (user_name = '%s' OR email = '%s') AND password = '%s'",
    login_info->username,
    login_info->email,
    login_info->password);
    printf("Voici la query de connexion : \n\n%s\n\n", query);
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
    

    if (rows > 0 && cols > 0)
    {
        char *result_str = PQgetvalue(res, 0, 0);
        query_result = atoi(result_str);  // Convert string to int
        
    }
    else {
        printf("erreur dans la requête, aucune colonne ou ligne trouvée ?");
    }
    
    printf("Query connexion : COUNT de username/email + password = %d", query_result);
    PQclear(res);

    PQfinish(conn);
    printf("\n\nBye bye\n\n");
    
    return query_result;
}

void first_update_client_package(Client_package *client_package) {
    printf("[first_update_client_package debut] CHECKPOINT: channels %p\n", client_package->channels);
    printf("first_update_client_package\n");
    if (!client_package || !client_package->login_info || !client_package->client) {
        fprintf(stderr, "Invalid client_package or its subfields\n");
        return;
    }

    client_package->current_channel = 1;
    get_user_data(client_package);
    get_channel_list(client_package);
    // printf("[first_update_client_package avant get_full_chat_content] CHECKPOINT: channels ptr = %p\n", client_package->channels);
    get_full_chat_content(client_package);
    // printf("[first_update_client_package fin] CHECKPOINT: channels %p\n", client_package->channels);
}

void get_user_data(Client_package* client_package) {
    if (!client_package || !client_package->login_info || !client_package->client) {
        fprintf(stderr, "Invalid client_package or its subfields\n");
        return;
    }
    // printf("[get_user_data begin] CHECKPOINT: login_info ptr = %p\n", client_package->login_info);
    printf("[get_user_data begin] CHECKPOINT: login_info ptr = %p\n channels %p\n", client_package->login_info, client_package->channels);

    PGconn *conn = database_connexion();
    PGresult *res = NULL;
    int nFields;

    int final_size_query = snprintf(NULL, 0,
        "SELECT * FROM users\n\
    WHERE (user_name = '%s' OR email = '%s') AND password = '%s'",
    client_package->login_info->username,
    client_package->login_info->email,
    client_package->login_info->password) +1;

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
    WHERE (user_name = '%s' OR email = '%s') AND password = '%s'",
    client_package->login_info->username,
    client_package->login_info->email,
    client_package->login_info->password);
    
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
        char *result_email = PQgetvalue(res, 0, 2);
        int user_id = atoi(result_user_id);  // Convert string to int
        client_package->login_info->user_id = user_id;
        client_package->client->user_id = user_id;
        
        strncpy(client_package->login_info->username, result_user_name, SMALL_STRING);
        client_package->login_info->username[SMALL_STRING -1] = '\0';
        strncpy(client_package->client->username, result_user_name, SMALL_STRING);
        client_package->client->username[SMALL_STRING -1] = '\0';
        strncpy(client_package->login_info->email, result_email, LARGE_PLUS_STRING);
        client_package->login_info->email[LARGE_PLUS_STRING -1] = '\0';
        // printf("print du login info après la requête : id %d, name %s\n", client_package->login_info->user_id, client_package->login_info->username);
    }
    else {
        printf("erreur dans la requête, aucune colonne ou ligne trouvée ?\n");
    }
    

    PQclear(res);

    PQfinish(conn);
    // printf("[get_user_data end] CHECKPOINT: login_info ptr = %p\n", client_package->login_info);
    printf("[get_user_data end] CHECKPOINT: login_info ptr = %p\n channels %p\n", client_package->login_info, client_package->channels);
    
    printf("\n\nBye bye\n\n");
}