#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H
#include <gtk/gtk.h>
#include "../../controller/header/utils.h"
#define STYLE_PATH "../../source/front/style.css"

void show_login_window(GtkApplication *app, gpointer user_data, Login_infos *login_info);
// TODO: Declare the validate_credentials function here once created
// bool validate_credentials(const gchar *username, const gchar *password);

#endif
