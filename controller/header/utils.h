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

typedef struct {
    int client_id;
    int channel_id;
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
    SOCKET sock_pointer;
    char client_name[64];
    int client_id;
    Channel_info *channels;

} Client_data;


typedef struct
{
    Client_data *clients[MAX_CLIENTS];
    pthread_mutex_t lock;
    int client_count;
} Server_state;

typedef struct {
    char username[64];
    char password[64];
    char confirm_password[64];
    char email[100];
    int login_register;
    int user_id;
    int current_channel;
} Login_infos;

typedef struct
{
    Client_data *client;
    Server_state *server;
    Message *message_list;// create to a message_list struct
    Message message_send; 
    Login_infos *login_info;
} Client_package;

typedef struct {
    Client_data *client;
    Message message_send;
    Login_infos *login_info;
    GtkWidget **data;
    GtkApplication *app;  
} Login_package_for_front;

#endif