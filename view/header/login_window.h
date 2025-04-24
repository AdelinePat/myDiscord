#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H
#include <gtk/gtk.h>
#define STYLE_PATH "./view/style.css"

void show_login_window(GtkApplication *app);
// TODO: Declare the validate_credentials function here once created
// bool validate_credentials(const gchar *username, const gchar *password);

#endif
