#ifndef UTILS_H
#define UTILS_H
#include <time.h>
#include <pthread.h>
#include <winsock2.h>
#include <gtk/gtk.h>

#define MAX_CLIENTS 20

struct tm get_timestamp();

typedef struct {
    char username[64];
    char password[64];
    char confirm_password[64];
    char email[100];
    int login_register;
} Login_infos;

typedef struct {
    int client_id;
    int channel_id;
    struct tm timestamp;
    char message[1024];
} Message;

typedef struct {
    SOCKET sock_pointer;
    char client_name[64];
    int client_id;
} Client_data;

typedef struct {
    Client_data *clients[MAX_CLIENTS];
    pthread_mutex_t lock;
    int client_count;
} Server_state;

typedef struct {
    Client_data *client;
    Server_state *server;
} Client_package;

typedef struct {
    Login_infos *login_info;
    GtkWidget **data;
} Login_package_for_front;

#endif