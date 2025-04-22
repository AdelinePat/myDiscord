#include <gtk/gtk.h>
#include "chat_window.h"

static void on_login_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget **data = user_data;
    GtkWidget *login_window = data[0];
    GtkWidget *entry_user = data[1];
    GtkWidget *entry_pass = data[2];
    GtkApplication *app = GTK_APPLICATION(data[3]);

    const gchar *username = gtk_entry_get_text(GTK_ENTRY(entry_user));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(entry_pass));

    g_print("Tentative de connexion: %s / %s\n", username, password);

     // TODO: Appeler la fonction validate_credentials() ici pour vérifier le format du mail/pseudo et mot de passe avec regex
    //       Ne pas spécifier si c'est le mail ou le mot de passe qui est incorrect
    // Ex : if (!validate_credentials(username, password)) { afficher une erreur générique et return; }

    gtk_widget_destroy(login_window); // ferme la fenêtre de connexion
    show_chat_window(app);           // lance le chat
}

static void on_register_clicked(GtkLabel *label, gpointer user_data) {
    (void)label;
    (void)user_data;
    g_print("Lien inscription cliqué !\n");
}

void show_login_window(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Connexion");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 20);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *entry_user = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_user), "Email ou Pseudo");
    gtk_box_pack_start(GTK_BOX(vbox), entry_user, FALSE, FALSE, 0);

    GtkWidget *entry_pass = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_pass), "Mot de passe");
    gtk_entry_set_visibility(GTK_ENTRY(entry_pass), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), entry_pass, FALSE, FALSE, 0);

    GtkWidget *btn_login = gtk_button_new_with_label("Se connecter");
    gtk_box_pack_start(GTK_BOX(vbox), btn_login, FALSE, FALSE, 0);

    GtkWidget *label_register = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_register), "<u><span foreground='blue'>Créer un compte</span></u>");
    gtk_widget_set_halign(label_register, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox), label_register, FALSE, FALSE, 0);

    GtkWidget **data = g_new(GtkWidget *, 4);  // allocation dynamique
    data[0] = window;
    data[1] = entry_user;
    data[2] = entry_pass;
    data[3] = GTK_WIDGET(app);

    g_signal_connect(btn_login, "clicked", G_CALLBACK(on_login_clicked), data);
    g_signal_connect(label_register, "button-press-event", G_CALLBACK(on_register_clicked), NULL);

    gtk_widget_show_all(window);
}
