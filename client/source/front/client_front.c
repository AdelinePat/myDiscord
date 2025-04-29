// #include <gtk/gtk.h>
#include "../../header/client_front.h"
// #include "./header/login_window.h"

static void on_activate(GtkApplication *app, gpointer user_data) {
    Login_package_for_front *login_pack = (Login_package_for_front *)user_data;
    // (void)user_data;
    show_login_window(login_pack);  // Start with the login screen
}

int launch_front() {
    GtkApplication *app = gtk_application_new("org.my.discord", G_APPLICATION_DEFAULT_FLAGS);

    Login_infos *login_info = malloc(sizeof(Login_infos));
    if (login_info == NULL) {
        g_warning("Failed to allocate memory for login_pack.");
        printf("Failed to allocate memory for login_pack."); // Or handle the error in an appropriate way
    }

    Login_package_for_front *login_pack = malloc(sizeof(Login_package_for_front));
    if (login_pack == NULL) {
        g_warning("Login package is NULL.");
        printf("login_pack est vide ? malloc failed");
    }

    login_pack->login_info = login_info;
    login_pack->app = app;
    
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), (void *)login_pack);
    int status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
    return status;
}
