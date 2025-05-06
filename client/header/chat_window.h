#ifndef CHAT_WINDOW_H
#define CHAT_WINDOW_H
#include "./client_front.h"
#include "../../controller/header/utils.h"

// void show_chat_window(GtkApplication *app);
void show_chat_window(Client_package_for_frontend *login_pack);
static void on_disconnect_clicked(GtkButton *button, gpointer user_data);
static void on_emoji_clicked(GtkButton *button, gpointer user_data);
static void on_chat_entry_activate(GtkEntry *entry, gpointer user_data);
static void on_emoji_button_clicked(GtkButton *button, gpointer user_data);
static void on_connect_clicked(GtkButton *button, gpointer user_data);

#endif
