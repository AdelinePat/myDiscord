#ifndef UTILS_H
#define UTILS_H
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <gtk/gtk.h>

#define MAX_CLIENTS 20

struct tm get_timestamp();
struct tm parse_db_query_time(char *time_str);
char * timestamp_to_char(struct tm time);

typedef enum
{
    LOGIN = 0,
    CREATE_ACCOUNT = 1,
    SWITCH_CHANNEL = 2,
    SEND_MESSAGE = 3,
    CREATE_CHANNEL = 4,
    SEND_REACTION = 5,
    LOGOUT = 6,
    CHANGE_ACCESS = 7,
    DELETE_ACCOUNT = 8
} SendType;

typedef struct {
    int channel_id;
    int message_id;
    int client_id;
    char username[19];
    struct tm timestamp;
    char message[999];
} Message;

typedef struct
{
    int channel_id;
    char channel_title[19];
} Channel_info;

typedef struct
{   
    SendType send_type;
    SOCKET sock_pointer;
    char client_name[19];
    int client_id; // client_id = user_id in database
    Channel_info *channels;

} Client_data;


typedef struct
{
    Client_data *clients[MAX_CLIENTS];
    pthread_mutex_t lock;
    int client_count;
} Server_state;

typedef struct {
    char username[19];
    char password[65];
    char confirm_password[65];
    char email[100];
    SendType login_register;
    int user_id;
    int current_channel;
} Login_infos;

typedef struct
{
    SendType send_type;
    Client_data *client;
    Server_state *server;
    Message *messages_list;// create to a message_list struct
    Message message_send; 
    Login_infos *login_info;
} Client_package;

typedef struct {
    SendType send_type;
    Client_data *client;
    Message *messages_list;// create to a message_list struct
    Message message_send;
    Login_infos *login_info;
    GtkWidget **data;
    GtkApplication *app;  
} Login_package_for_front;

#endif