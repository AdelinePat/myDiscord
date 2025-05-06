#include "../header/db_chat_content.h"
#include "../header/db_connection.h"
#include "../header/database_communication.h"

PGresult* generate_channel_list_query(PGconn *conn, Client_package *client_package)
{
    int final_size_query = snprintf(NULL, 0, "SELECT channel_id, channel_title\n\
        FROM channels\n\
        JOIN channels_access USING(channel_id)\n\
        WHERE user_id = '%d' AND role_title != 'ban'",
        client_package->login_info->user_id) +1;
    
    char *query = malloc(final_size_query);
    if (query == NULL) {
        fprintf(stderr, "Memory allocation failed for query.\n");
        PQfinish(conn);
        exit(1);
    }

    snprintf(query, final_size_query, "SELECT channel_id, channel_title\n\
        FROM channels\n\
        JOIN channels_access USING(channel_id)\n\
        WHERE user_id = '%d' AND role_title != 'ban'",
        client_package->login_info->user_id);

    PGresult *res = PQexec(conn, query);
    free(query);

    ExecStatusType resStatus = PQresultStatus(res);
    if (resStatus != PGRES_TUPLES_OK)
    {
        printf("il y a eu une erreur dans la requête de SELECT des channels");
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
    client_package->channel_list_size = rows;

    printf("\n");

    Channel_info *channels = malloc(sizeof(Channel_info)*rows);
    client_package->channels = channels;

    printf("user_id %d, user_name %s\n",
        client_package->login_info->user_id,
        client_package->login_info->username);

    if (rows > 0 && cols > 0)
    {
        printf("rows : %d, cols : %d\n", rows, cols);
        for (int row = 0; row < rows; row++)
            {
                client_package->channels[row].channel_id = atoi(PQgetvalue(res, row, 0));
                strcpy(client_package->channels[row].channel_title, PQgetvalue(res, row, 1));
                client_package->channels[row].channel_title[SMALL_STRING -1] = '\0';

                printf("Valeur row n°%d : %d %s\n",
                    row,
                    client_package->channels[row].channel_id,
                    client_package->channels[row].channel_title);

            }
    }
    else {
        printf("erreur dans la requête, aucune colonne ou ligne trouvée ?\n");
    }   

    PQclear(res);

    PQfinish(conn);
}

PGresult* generate_full_chat_content_query(PGconn *conn, Client_package *client_package)
{
    int final_size_query = snprintf(NULL, 0, "SELECT m.message_id, u.user_id,\n\
        COALESCE(u.user_name, 'Anonymous user'), m.date_time, m.content\n\
        FROM messages AS m\n\
        JOIN channels AS c ON c.channel_id = m.channel_id\n\
        LEFT JOIN users AS u ON u.user_id = m.user_id\n\
        WHERE c.channel_id = %d\n\
        ORDER BY date_time",
        client_package->current_channel) +1;
    
    char *query = malloc(final_size_query);
    if (query == NULL) {
        fprintf(stderr, "Memory allocation failed for query.\n");
        PQfinish(conn);
        exit(1);
    }

    snprintf(query, final_size_query, "SELECT m.message_id, u.user_id,\n\
        COALESCE(u.user_name, 'Anonymous user'), m.date_time, m.content\n\
        FROM messages AS m\n\
        JOIN channels AS c ON c.channel_id = m.channel_id\n\
        LEFT JOIN users AS u ON u.user_id = m.user_id\n\
        WHERE c.channel_id = %d\n\
        ORDER BY date_time",
        client_package->current_channel);

    PGresult *res = PQexec(conn, query);
    free(query);

    ExecStatusType resStatus = PQresultStatus(res);
    if (resStatus != PGRES_TUPLES_OK)
    {
        printf("il y a eu une erreur dans la requête pour récupérer le contenu du chat");
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }
    printf("query ok\n");
    return res;
}

int check_channel_access(PGconn *conn, Client_package *client_package)
{
    int access_granted = 0;
    int final_size_query = snprintf(NULL, 0, "SELECT role_title\n\
        FROM channels_access\n\
        WHERE user_id = %d AND channel_id = %d;",
        client_package->login_info->user_id,
        client_package->current_channel) +1;
    
    char *query = malloc(final_size_query);
    if (query == NULL) {
        fprintf(stderr, "Memory allocation failed for query.\n");
        PQfinish(conn);
        exit(1);
    }

    snprintf(query, final_size_query, "SELECT role_title\n\
        FROM channels_access\n\
        WHERE user_id = %d AND channel_id = %d;", client_package->login_info->user_id,
        client_package->current_channel);

    PGresult *res = PQexec(conn, query);
    free(query);

    ExecStatusType resStatus = PQresultStatus(res);
    if (resStatus != PGRES_TUPLES_OK)
    {
        printf("il y a eu une erreur dans la requête qui vérifie l'autorisation d'acces au channel");
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }

    int rows = PQntuples(res);
    if (rows == 1)
    {
        if (PQgetvalue(res, 0, 0) == 'ban')
        {
            access_granted = 0;
        }
        access_granted = 1;
    } else {
        access_granted = 0;
    }
    PQclear(res);
    PQfinish(conn);
    printf("access_granted = %d\n", access_granted);
    return access_granted;
 
}

void get_full_chat_content(Client_package *client_package) {
    PGconn *conn = database_connexion();
    int nFields;
    printf("\n\n\tdans get_full_chat_content\n");

    if (check_channel_access(conn, client_package) == 1)
    {
        conn = database_connexion();
        PGresult *res = generate_full_chat_content_query(conn, client_package);
        int rows = PQntuples(res);
        int cols = PQnfields(res);
        client_package->number_of_messages = rows;

        printf("\n");

        Message *messages_list = malloc(sizeof(Message)*rows);
        client_package->messages_list = messages_list;

        printf("user_id %d, current_channel_id %d\n",
            client_package->login_info->user_id,
            client_package->current_channel);
            
        if (rows > 0 && cols > 0)
        {
            printf("rows : %d, cols : %d\n", rows, cols);
            for (int row = 0; row < rows; row++)
                {
                    client_package->messages_list[row].channel_id = client_package->current_channel;
                    client_package->messages_list[row].message_id = atoi(PQgetvalue(res, row, 0));
                    client_package->messages_list[row].user_id = atoi(PQgetvalue(res, row, 1)); // user_id = client_id ??
                    
                    strncpy(client_package->messages_list[row].username, PQgetvalue(res, row, 2), SMALL_STRING);
                    client_package->messages_list[row].username[SMALL_STRING -1] = '\0';
                    
                    char *time_db = PQgetvalue(res, row, 3);
                    struct tm time_db_parsed = parse_db_query_time(time_db);
                    char * formated_timestamp = timestamp_to_char(time_db_parsed);
                    
                    strncpy(client_package->messages_list[row].timestamp, formated_timestamp, MEDIUM_STRING);
                    client_package->messages_list[row].timestamp[MEDIUM_STRING -1] = '\0';

                    free(formated_timestamp);

                    strncpy(client_package->messages_list[row].message, PQgetvalue(res, row, 4), VERY_LARGE_STRING);
                    client_package->messages_list[row].message[VERY_LARGE_STRING -1] = '\0';

                    printf("Valeur row n°%d : message_id %d, user_id %d:\tusername %s\t%s\n, content : %s\n",
                        row,
                        client_package->messages_list[row].message_id,
                        client_package->messages_list[row].user_id,
                        client_package->messages_list[row].username,
                        client_package->messages_list[row].timestamp,
                        client_package->messages_list[row].message);
                }
        }
        else {

            printf("erreur dans la requête, aucune colonne ou ligne trouvée ?\n");
        }   

        PQclear(res);
        PQfinish(conn);
    }
    printf("\n\nRequete get_full_chat_content terminee\n\n");
}