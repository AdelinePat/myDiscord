#ifndef REGISTER_WINDOW_H
#define REGISTER_WINDOW_H
// #include <gtk/gtk.h>
#include "./client_front.h"
#include "./login_window.h"
#include "../../controller/header/utils.h"
#include "../header/login_window.h"

// === FUNCTION DECLARATION: Display the Registration Window ===
// This function creates and shows the registration window.
// It allows the user to enter their username, email, password, and confirm password.
// void show_register_window(GtkApplication *app);
void show_register_window(Login_package_for_front *login_pack);
static void on_confirm_clicked(GtkButton *button, gpointer user_data);
static void on_return_clicked(GtkButton *button, gpointer user_data);

#endif // REGISTER_WINDOW_H
