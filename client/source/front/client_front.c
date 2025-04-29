#include <gtk/gtk.h>
#include "../../header/login_window.h"
#include "../../header/client_front.h"

// static void on_activate(GtkApplication *app, gpointer user_data) {
//     (void)user_data;
//     show_login_window(app);  // Start with the login screen
// }

static void on_activate(Login_package_for_front *login_pack) {
    printf("5. Dans le callback\n");
    // (void)user_data;
    // GtkApplication *app = login_pack->app;
    // gpointer user_data = login_pack->data;
    // Login_infos login_info = login_pack->login_info;
    if (login_pack == NULL) {
        g_warning("Login package is NULL.");
        printf("DANS ON ACTIVATE : login_pack est vide ? malloc failed");
    }
    printf("Je print l'app via login_pack dans on_activate : %d\n", login_pack->app);
    show_login_window(login_pack); // Start with the login screen
    printf("X. Après show_login_window\n");
}

int launch_front(Login_infos *login_info) {
    printf("1. je suis dans launch front avant de démarrer l'application\n");
    GtkApplication *app = gtk_application_new("org.my.discord", G_APPLICATION_DEFAULT_FLAGS);
    printf("2. j'ai tenté d'instancier gtk_application_new\n");

    gpointer user_data = NULL;
    Login_package_for_front *login_pack = malloc(sizeof(Login_package_for_front));
    if (login_pack == NULL) {
        g_warning("Login package is NULL.");
        printf("login_pack est vide ? malloc failed");
    }

    printf("3. gpointer créé ? login_package_for_front créé ?\n");

    if (login_pack == NULL) {
        g_warning("Failed to allocate memory for login_pack.");
        printf("Failed to allocate memory for login_pack."); // Or handle the error in an appropriate way
    }

    login_pack->login_info = login_info;
    // GtkApplication *app = login_pack->app;
    login_pack->app = app;
    printf("Je print l'app native : %d\n", app);
    printf("Je print l'app via login_pack : %d\n", login_pack->app);
    // login_pack->data = user_data;
    // GtkWidget **data = login_pack->data;
    login_pack->data = NULL;

    printf("4. Juste avant le callback\n");
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), (void *)login_pack);
    printf("Après le callback'\n");
    // int status = g_application_run(G_APPLICATION(app), argc, argv);
    int status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
    return status;
}
