#ifndef REGISTER_WINDOW_H
#define REGISTER_WINDOW_H

#include <gtk/gtk.h>
#include "login_window.h"

// === FUNCTION DECLARATION: Display the Registration Window ===
// This function creates and shows the registration window.
// It allows the user to enter their username, email, password, and confirm password.
void show_register_window(GtkApplication *app);

#endif // REGISTER_WINDOW_H
