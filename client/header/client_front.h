#ifndef CLIENT_FRONT
#define CLIENT_FRONT
#define STYLE_PATH "./client/source/front/style.css"
// #include <winsock2.h>
#include "./login_window.h"
#include "../../controller/header/utils.h"
#include "../header/connector.h"
#include "login_window.h"

static void on_activate(GtkApplication *app, gpointer user_data);
int launch_front();

#endif