#include "../header/save_message.h"
#include "../header/register_user.h"

PGresult * create_new_message_query(PGconn *conn, Client_package *client_package)
{
    PGresult *res = NULL;
    int nFields;
    int final_size_query = snprintf(NULL, 0,
        "INSERT INTO messages (user_id, channel_id, content)\n\
        VALUES (%d, %d, '%s');", 
        client_package->login_info->user_id,
        client_package->current_channel,
        client_package->message_send.message) +1;

    char *query = malloc(final_size_query);
    if (query == NULL) {
        fprintf(stderr, "Memory allocation failed for query.\n");
        PQfinish(conn);
        exit(1);
    }
    snprintf(query, final_size_query,
        "INSERT INTO messages (user_id, channel_id, content)\n\
        VALUES (%d, %d, '%s');", 
        client_package->login_info->user_id,
        client_package->current_channel,
        client_package->message_send.message);
    
    printf("\n\nprint query create message \n%s\n\n", query);
    res = PQexec(conn, query);
    free(query);
    ExecStatusType resStatus = PQresultStatus(res);

    if (resStatus != PGRES_COMMAND_OK)
    {
        printf("il y a eu une erreur dans la requête de sauvegarde du message dans la table");
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }
    return res;
}

void create_new_message_db(Client_package* client_package) {
    printf("\n\n\ncreate new message\n\n\n\n");
    PGconn *conn = database_connexion();
    PGresult *res = NULL;
    int nFields;

    if(does_user_exists(client_package->login_info) != 1)
    {
        printf("Impossible d'envoyer un message avec un utilisateur inexistant");
    } else {
        res = create_new_message_query(conn, client_package);
        printf("creation du nouveal message = SUCCES");       
        PQclear(res);
        PQfinish(conn);
    }
}