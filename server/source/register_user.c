#include "../header/register_user.h"

int does_user_exists(Login_infos* login_info) {
    PGconn *conn = database_connexion();
    PGresult *res = NULL;
    int nFields;

    int final_size_query = snprintf(NULL, 0,
        "SELECT COUNT(user_name) FROM users\n\
    WHERE (user_name = '%s' OR email = '%s')",
    login_info->username,
    login_info->email) +1;

    char *query = malloc(final_size_query);
    if (query == NULL) {
        fprintf(stderr, "Memory allocation failed for query.\n");
        PQfinish(conn);
        exit(1);
    }

    printf("valeur de user_name <<%s>> et de email <<%s>> et password : <<%s>> avant la requete\n",
        login_info->username,
        login_info->email,
        login_info->password);
        
    snprintf(query, final_size_query,
        "SELECT COUNT(user_name) FROM users\n\
    WHERE (user_name = '%s' OR email = '%s')", 
    login_info->username,
    login_info->email);

    res = PQexec(conn, query);
    free(query);

    ExecStatusType resStatus = PQresultStatus(res);
    
    if (resStatus != PGRES_TUPLES_OK)
    {
        printf("il y a eu une erreur dans la requête qui compte le nombre d'utilisateur qui corresponde au username ou email transmis par Gtk");
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }

    int rows = PQntuples(res);
    int cols = PQnfields(res);
    int query_result = 0;
    printf("nombre de rows : %d\n", rows);
    if (rows > 0)
    {
        char *result_str = PQgetvalue(res, 0, 0);
        query_result = atoi(result_str);  // Convert string to int
        printf("resultat query user exist : %d\n", query_result);
    }    

    PQclear(res);

    PQfinish(conn);
    
    return query_result;
}

PGresult * create_new_user_query(PGconn *conn, Client_package *client_package)
{
    PGresult *res = NULL;
    int nFields;
    int final_size_query = snprintf(NULL, 0,
        "INSERT INTO users (user_name, email, password)\n\
        VALUES('%s', '%s', '%s');", client_package->login_info->username, client_package->login_info->email, client_package->login_info->password) +1;

    char *query = malloc(final_size_query);
    if (query == NULL) {
        fprintf(stderr, "Memory allocation failed for query.\n");
        PQfinish(conn);
        exit(1);
    }
    snprintf(query, final_size_query,
        "INSERT INTO users (user_name, email, password)\n\
        VALUES('%s', '%s', '%s');", client_package->login_info->username, client_package->login_info->email, client_package->login_info->password);

    res = PQexec(conn, query);
    free(query);
    ExecStatusType resStatus = PQresultStatus(res);

    if (resStatus != PGRES_COMMAND_OK)
    {   
        fprintf(stderr, "ERROR: %s\n", PQerrorMessage(conn));
        printf("il y a eu une erreur dans la requête de création de user\n");
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }
    return res;
}

void create_new_user_db(Client_package* client_package) {
    PGconn *conn = database_connexion();
    PGresult *res = NULL;
    int nFields;

    if(does_user_exists(client_package->login_info) != 0)
    {
        printf("Impossible de créer un utilisateur qui existe déjà\n");
    } else {
        res = create_new_user_query(conn, client_package);
        printf("creation du nouveal utilisateur = SUCCES\n");       
        PQclear(res);
        PQfinish(conn);
    }
}