#include <gtk/gtk.h>


#include "login_window.h"

static void on_activate(GtkApplication *app, gpointer user_data) {
    (void)user_data;
    show_login_window(app);  // commence par l'écran de login
}


int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.my.discord", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
