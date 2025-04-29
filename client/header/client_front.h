#ifndef CLIENT_FRONT
#define CLIENT_FRONT
#include <gtk/gtk.h>
#include "../header/login_window.h"
#include "../../controller/header/utils.h"

// static void on_activate(GtkApplication *app, gpointer user_data, Login_infos *login_info);
static void on_activate(GtkApplication *app, gpointer user_data);
int launch_front(Login_infos *login_info);

#endif