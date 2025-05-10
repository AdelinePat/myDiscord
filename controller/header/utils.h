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
#define SMALL_STRING 20
#define MEDIUM_STRING 32
#define LARGE_STRING 65
#define LARGE_PLUS_STRING 100
#define VERY_LARGE_STRING 1000
#define MAX_CLIENTS 20

char * get_str_timestamp();
struct tm parse_db_query_time(char *time_str);
char * timestamp_to_char(struct tm time);

// char * debug_get_str_timestamp();
void debug_get_str_timestamp(char * buffer, size_t buffer_size);
char * debug_timestamp_to_char(struct tm time);

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
    SOCKET_FAIL = 10,
    READY = 11,
    WAIT = 12,
    MESSAGE_REFRESHED = 13
} SendType;

typedef struct {
    int channel_id;
    int message_id;
    int user_id;
    char username[SMALL_STRING];
    char timestamp[MEDIUM_STRING];
    char message[VERY_LARGE_STRING];
} Message;

typedef struct
{
    int channel_id;
    char channel_title[SMALL_STRING];
} Channel_info;

typedef struct
{   
    SOCKET sock_pointer;
    char username[SMALL_STRING];
    int user_id; // client_id = user_id in database
} Client_data;

typedef struct
{
    Client_data *clients[MAX_CLIENTS];
    pthread_mutex_t lock;
    int client_count;
} Server_state;

typedef struct {
    char username[SMALL_STRING];
    char password[LARGE_STRING];
    char confirm_password[LARGE_STRING];
    char email[LARGE_PLUS_STRING];
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
    char * client_pack_str;
    int size_string;
} Client_package;

typedef struct {
    Client_package *client_package;
    Server_state *server;
} Client_package_for_backend;

typedef struct {
    Client_package *client_package;
    GtkWidget **data;
    GtkApplication *app;
    GtkWidget *chat_display;
    GtkWidget *window;

} Client_package_for_frontend;

    // SendType send_type;
    // Client_data *client;
    // Message *messages_list;// create to a message_list struct;
    // Message message_send;
    // int number_of_messages;
    // Login_infos *login_info;
char * serialize_client_package(Client_package *client_package);
void serialize_message_list(cJSON * message_list_json, Client_package * client_package, int * size_string);
cJSON * serialize_message(Message * a_message, int * size_string);
void serialize_channel_info_list(cJSON * channel_list, Client_package * client_package, int * size_string);
cJSON * serialize_channel_info(Channel_info channel_info, int * size_string);
void serialize_client_data(cJSON * clientData, Client_data *client, int * size_string);
void serialize_login_info(cJSON * login, Login_infos *login_info);

cJSON * parse_login_info(cJSON *root, Client_package * client_package); 
cJSON * parse_client_data(cJSON *root, Client_package * client_package);
void parse_message_list(cJSON *root, Client_package * client_package);
void parse_a_message(cJSON * a_message, Client_package * client_package, int index);
void parse_channels_info_list(cJSON *root, Client_package * client_package);
void parse_client_package_str(Client_package * client_package, char * client_package_str);

#endif