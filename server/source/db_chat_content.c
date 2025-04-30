#include "../header/db_chat_content.h"
#include "../header/db_connection.h"
#include "../header/database_communication.h"


void get_channel_list(Client_package *client_package) {
    PGconn *conn = database_connexion();
    PGresult *res = NULL;
    int nFields;

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

    // char* query_result = 0;
    // printf("value du result de la query mdr : %d", query_result);

    if (rows > 0 && cols > 0)
    {
        for (int y = 0; y < rows; y++)
            {
                for (int j = 0; j < cols ; j++)
                {
                    client_package->client->channels[y] = PQgetvalue(res, y, j);
                    printf("ce que j'ajoute à channels[y] y = %d et value : %s\n", y, PQgetvalue(res, y, j));
                }
                printf("\n");
            }
    }
    else {
        printf("erreur dans la requête, aucune colonne ou ligne trouvée ?\n");
    }
    

    PQclear(res);

    PQfinish(conn);
    printf("\n\nBye bye\n\n");
}