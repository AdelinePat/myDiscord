#include "../header/db_chat_content.h"
#include "../header/db_connection.h"
#include "../header/database_communication.h"

PGresult* generate_channel_list_query(PGconn *conn, Client_package *client_package)
{
    int final_size_query = snprintf(NULL, 0, "SELECT channel_id, channel_title\n\
        FROM channels\n\
        JOIN channels_access USING(channel_id)\n\
        WHERE user_id = '%d' AND role_title != 'ban'", client_package->login_info->user_id) +1;
    
    char *query = malloc(final_size_query);
    if (query == NULL) {
        fprintf(stderr, "Memory allocation failed for query.\n");
        PQfinish(conn);
        exit(1);
    }

    snprintf(query, final_size_query, "SELECT channel_id, channel_title\n\
        FROM channels\n\
        JOIN channels_access USING(channel_id)\n\
        WHERE user_id = '%d' AND role_title != 'ban'", client_package->login_info->user_id);

    PGresult *res = PQexec(conn, query);
    free(query);

    ExecStatusType resStatus = PQresultStatus(res);
    if (resStatus != PGRES_TUPLES_OK)
    {
        printf("il y a eu une erreur dans la requête");
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }
    return res;
}

void get_channel_list(Client_package *client_package) {
    PGconn *conn = database_connexion();
    int nFields;

    PGresult *res = generate_channel_list_query(conn, client_package);

    int rows = PQntuples(res);
    int cols = PQnfields(res);

    printf("\n");

    Channel_info *channels = malloc(sizeof(Channel_info)*rows);
    client_package->client->channels = channels;

    printf("user_id %d, user_name %s\n", client_package->login_info->user_id, client_package->login_info->username);
    if (rows > 0 && cols > 0)
    {
        printf("rows : %d, cols : %d\n", rows, cols);
        for (int row = 0; row < rows; row++)
            {
                client_package->client->channels[row].channel_id = atoi(PQgetvalue(res, row, 0));
                strcpy(client_package->client->channels[row].channel_title, PQgetvalue(res, row, 1));

                printf("Valeur row n°%d : %d %s\n", row, client_package->client->channels[row].channel_id, client_package->client->channels[row].channel_title);

            }
    }
    else {
        printf("erreur dans la requête, aucune colonne ou ligne trouvée ?\n");
    }   

    PQclear(res);

    PQfinish(conn);
    printf("\n\nBye bye\n\n");
}

PGresult* generate_full_chat_content_query(PGconn *conn, Client_package *client_package)
{
    return 1;
}

void get_full_chat_content(Client_package *client_package) {
    PGconn *conn = database_connexion();
    int nFields;

    PGresult *res = generate_full_chat_content_query(conn, client_package);

    int rows = PQntuples(res);
    int cols = PQnfields(res);

    printf("\n");

    Channel_info *channels = malloc(sizeof(Channel_info)*rows);
    client_package->client->channels = channels;

    printf("user_id %d, user_name %s\n", client_package->login_info->user_id, client_package->login_info->username);
    if (rows > 0 && cols > 0)
    {
        printf("rows : %d, cols : %d\n", rows, cols);
        for (int row = 0; row < rows; row++)
            {
                client_package->client->channels[row].channel_id = atoi(PQgetvalue(res, row, 0));
                strcpy(client_package->client->channels[row].channel_title, PQgetvalue(res, row, 1));

                printf("Valeur row n°%d : %d %s\n", row, client_package->client->channels[row].channel_id, client_package->client->channels[row].channel_title);

            }
    }
    else {
        printf("erreur dans la requête, aucune colonne ou ligne trouvée ?\n");
    }   

    PQclear(res);

    PQfinish(conn);
    printf("\n\nBye bye\n\n");
}