#include <gtk/gtk.h>
#include "../../header/login_window.h"
#include "../../header/client_front.h"

// static void on_activate(GtkApplication *app, gpointer user_data) {
//     (void)user_data;
//     show_login_window(app);  // Start with the login screen
// }

static void on_activate(Login_package_for_front *login_pack) {
    // (void)user_data;
    // GtkApplication *app = login_pack->app;
    // gpointer user_data = login_pack->data;
    // Login_infos login_info = login_pack->login_info;
    show_login_window(login_pack);  // Start with the login screen
}

int launch_front(Login_infos *login_info) {
    GtkApplication *app = gtk_application_new("org.my.discord", G_APPLICATION_DEFAULT_FLAGS);

    gpointer user_data = NULL;
    Login_package_for_front *login_pack = malloc(sizeof(Login_package_for_front));
    if (login_pack == NULL) {
        g_warning("Failed to allocate memory for login_pack.");
        printf("Failed to allocate memory for login_pack."); // Or handle the error in an appropriate way
    }
    
    login_pack->login_info = login_info;
    login_pack->app = app;
    login_pack->data = user_data;

    g_signal_connect(app, "activate", G_CALLBACK(on_activate), login_pack);
    // int status = g_application_run(G_APPLICATION(app), argc, argv);
    int status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
    return status;
}
