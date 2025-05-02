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
#include <cjson/cJSON.h>

#define MAX_CLIENTS 20

char * get_str_timestamp();
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
    DELETE_ACCOUNT = 8,
    SOCKET_OK = 9,
    SOCKET_FAIL = 10
} SendType;

typedef struct {
    int channel_id;
    int message_id;
    int client_id;
    char username[19];
    char timestamp[30];
    char message[999];
} Message;

typedef struct
{
    int channel_id;
    char channel_title[19];
} Channel_info;

typedef struct
{   
    SOCKET sock_pointer;
    char client_name[19];
    int user_id; // client_id = user_id in database
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
    int user_id;
} Login_infos;

typedef struct {
    SOCKET *sock_copy;
    GtkWidget *chat_display;
} Chat_display_package;

typedef struct {
    int send_type; // value of SendType Struct
    Client_data *client; //
    Message *messages_list;// create to a message_list struct
    Message message_send; 
    int number_of_messages; //
    Channel_info *channels;
    int channel_list_size; //
    int current_channel; //
    Login_infos *login_info;
} Client_package;

typedef struct {
    Client_package *client_package;
    Server_state *server;
} Client_package_for_backend;

typedef struct {
    Client_package *client_package;
    GtkWidget **data;
    GtkApplication *app;  
} Client_package_for_frontend;

    // SendType send_type;
    // Client_data *client;
    // Message *messages_list;// create to a message_list struct;
    // Message message_send;
    // int number_of_messages;
    // Login_infos *login_info;
char * serialize_client_package(Client_package *client_package);
void serialize_login_info(cJSON *login, Login_infos *login_info);

#endif