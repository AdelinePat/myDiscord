#include <stdlib.h>
#include <stdio.h>
#include <libpq-fe.h>
// #include <confuse.h>
// #define ENV_PATH "C:\\Users\\Adeline\\Documents\\Projetdev\\myDiscord\\.env"

int main(int argc, char const *argv[])
{  
    // const char *file = getenv(ENV_PATH)
    // char user_name[] = "";
    // char user_password[] = "";
    const char conninfo[] = "dbname = whispr user=user_name password=user_password host=localhost port=5432";
    PGconn *conn;
    PGresult *res;
    int nFields;
    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK)
    {
        printf("Erreur lors de la connexion");
        PQfinish(conn);
        exit(1);
    }

    printf("Connexion établie,\nport utilisé %s\nhostname %s\nDBname : %s", PQport(conn), PQhost(conn), PQdb(conn));
    
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
    
    char *query = "SELECT c.channel_title, COALESCE(u.user_name, 'Anonymous user'), m.date_time, m.content "
        "FROM messages AS m "
        "JOIN channels AS c ON c.channel_id = m.channel_id "
        "LEFT JOIN users AS u ON u.user_id = m.user_id "
        "WHERE c.channel_id IN ( "
            "SELECT channel_id FROM channels "
            "WHERE channel_status = 'private' "
        ") AND u.user_id IN ( "
            "SELECT user_id "
            "FROM channels_access "
        " WHERE role_title != 'ban' AND channel_id = 3)";

    // multiple line query add \n\ at the end of every line OR quote at the begging of every line

    res = PQexec(conn, query);

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
    for (int i = 0; i < cols; i++)
    {
        printf("%s\t", PQfname(res, i));
    }
    printf("\n");

    for (int y = 0; y < rows; y++)
    {
        for (int j = 0; j < cols ; j++)
        {
            printf("%s\t", PQgetvalue(res, y, j));
        }
        printf("\n");
    }

    PQclear(res);

    PQfinish(conn);
    printf("\n\nBye bye\n\n");
    return 0;
}
