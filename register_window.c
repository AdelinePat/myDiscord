#include <gtk/gtk.h>
#include "login_window.h"
#include "register_window.h"

static void on_confirm_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget **data = user_data;
    GtkWidget *register_window = data[0];
    GtkWidget *entry_user = data[1];
    GtkWidget *entry_email = data[2];
    GtkWidget *entry_pass = data[3];
    GtkWidget *entry_confirm_pass = data[4];
    GtkApplication *app = GTK_APPLICATION(data[5]);

    const gchar *username = gtk_entry_get_text(GTK_ENTRY(entry_user));
    const gchar *email = gtk_entry_get_text(GTK_ENTRY(entry_email));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(entry_pass));
    const gchar *confirm_password = gtk_entry_get_text(GTK_ENTRY(entry_confirm_pass));

    // Vérifie que les mots de passe sont identiques
    if (g_strcmp0(password, confirm_password) != 0) {
        g_print("Les mots de passe ne correspondent pas !\n");
        return;
    }

    g_print("Inscription avec: %s / %s / %s\n", username, email, password);

    // TODO: Appeler la fonction validate_credentials() pour vérifier les champs

    gtk_widget_destroy(register_window); // ferme la fenêtre d'inscription
    show_login_window(app);              // retourne à la fenêtre de connexion
}

static void on_return_clicked(GtkButton *button, gpointer user_data) {
    GtkApplication *app = GTK_APPLICATION(user_data);
    show_login_window(app);  // retourne à la fenêtre de connexion
}

void show_register_window(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Inscription");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 350);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 20);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *entry_user = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_user), "Pseudo");
    gtk_box_pack_start(GTK_BOX(vbox), entry_user, FALSE, FALSE, 0);

    GtkWidget *entry_email = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_email), "Email");
    gtk_box_pack_start(GTK_BOX(vbox), entry_email, FALSE, FALSE, 0);

    GtkWidget *entry_pass = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_pass), "Mot de passe");
    gtk_entry_set_visibility(GTK_ENTRY(entry_pass), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), entry_pass, FALSE, FALSE, 0);

    GtkWidget *entry_confirm_pass = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_confirm_pass), "Confirmer le mot de passe");
    gtk_entry_set_visibility(GTK_ENTRY(entry_confirm_pass), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), entry_confirm_pass, FALSE, FALSE, 0);

    // Crée une boîte horizontale pour les boutons
    GtkWidget *hbox_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); // espacement entre les boutons
    gtk_box_pack_start(GTK_BOX(vbox), hbox_buttons, FALSE, FALSE, 0);

    // Crée les boutons
    GtkWidget *btn_confirm = gtk_button_new_with_label("Confirmer");
    GtkWidget *btn_return = gtk_button_new_with_label("Retour");

    // Ajoute les boutons dans la hbox
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_confirm, TRUE, TRUE, 5); 
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_return, TRUE, TRUE, 5);

    // Allocation des données pour les passer à la fonction de signal
    GtkWidget **data = g_new(GtkWidget *, 6);  // allocation dynamique
    data[0] = window;
    data[1] = entry_user;
    data[2] = entry_email;
    data[3] = entry_pass;
    data[4] = entry_confirm_pass;
    data[5] = GTK_WIDGET(app);

    g_signal_connect(btn_confirm, "clicked", G_CALLBACK(on_confirm_clicked), data);
    g_signal_connect(btn_return, "clicked", G_CALLBACK(on_return_clicked), app);

    gtk_widget_show_all(window);
}
