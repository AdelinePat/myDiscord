#include <gtk/gtk.h>
#include "../../header/login_window.h"
#include "../../header/client_front.h"

// static void on_activate(GtkApplication *app, gpointer user_data) {
//     (void)user_data;
//     show_login_window(app);  // Start with the login screen
// }

static void on_activate(GtkApplication *app, gpointer user_data, Login_infos *login_info) {
    // (void)user_data;
    show_login_window(app, user_data, login_info);  // Start with the login screen
}

int launch_front(Login_infos *login_info) {
    GtkApplication *app = gtk_application_new("org.my.discord", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    // int status = g_application_run(G_APPLICATION(app), argc, argv);
    int status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
    return status;
}
