#ifndef REGISTER_WINDOW_H
#define REGISTER_WINDOW_H
#include "login_window.h"
#include <gtk/gtk.h>

// === FUNCTION DECLARATION: Display the Registration Window ===
// This function creates and shows the registration window.
// It allows the user to enter their username, email, password, and confirm password.
void show_register_window(GtkApplication *app,  gpointer user_data, Login_infos *login_info);

#endif // REGISTER_WINDOW_H
