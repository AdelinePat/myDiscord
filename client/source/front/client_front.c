#include "../../header/client_front.h"

static void on_activate(GtkApplication *app, gpointer user_data) {
    Client_package_for_frontend *login_pack = (Client_package_for_frontend *)user_data;

    login_pack->client_package->client->sock_pointer = client_start(); // OK
    printf("\non a reçu la socket client après connexion ??? %lld\n", login_pack->client_package->client->sock_pointer);
    
    connect_to_server(login_pack);
    show_login_window(login_pack);  // Start with the login screen
}

int launch_front() {
    GtkApplication *app = gtk_application_new("org.my.discord", G_APPLICATION_DEFAULT_FLAGS);

    Client_package_for_frontend *login_pack = malloc(sizeof(Client_package_for_frontend));

    if (login_pack == NULL) {
        g_warning("Login package is NULL.");
        printf("login_pack est vide ? malloc failed");
    }
    login_pack->client_package = malloc(sizeof(Client_package));
    if (login_pack->client_package == NULL) {
        g_warning("Failed to allocate memory for login_pack.");
        printf("Failed to allocate memory for login_pack->client."); // Or handle the error in an appropriate way
    }
    login_pack->client_package->client = malloc(sizeof(Client_data));
    if (login_pack->client_package->client == NULL) {
        g_warning("Failed to allocate memory for login_pack.");
        printf("Failed to allocate memory for login_pack->client."); // Or handle the error in an appropriate way
    }

    Login_infos *login_info = malloc(sizeof(Login_infos));
    login_pack->client_package->login_info = login_info;
    if (login_pack->client_package->login_info == NULL) {
        g_warning("Failed to allocate memory for login_pack.");
        printf("Failed to allocate memory for login_pack->login_info."); // Or handle the error in an appropriate way
    }
    
    login_pack->app = app;
    
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), (void *)login_pack);
    int status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);

    return status;
}
