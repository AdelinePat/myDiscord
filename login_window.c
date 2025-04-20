#include <gtk/gtk.h>

static void on_login_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget **entries = user_data;
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(entries[1]));

    // Logique de connexion ici (à remplacer plus tard)
    g_print("Tentative de connexion avec: %s / %s\n", username, password);

    // TODO: Vérifier utilisateur et passer à la fenêtre de chat
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
    gtk_entry_set_visibility(GTK_ENTRY(entry_pass), FALSE);  // Masque le texte
    gtk_box_pack_start(GTK_BOX(vbox), entry_pass, FALSE, FALSE, 0);

    GtkWidget *btn_login = gtk_button_new_with_label("Se connecter");
    gtk_box_pack_start(GTK_BOX(vbox), btn_login, FALSE, FALSE, 0);

    GtkWidget *label_register = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label_register), "<u><span foreground='blue'>Créer un compte</span></u>");
    gtk_widget_set_halign(label_register, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox), label_register, FALSE, FALSE, 0);

    GtkWidget *entries[2] = { entry_user, entry_pass };
    g_signal_connect(btn_login, "clicked", G_CALLBACK(on_login_clicked), entries);
    g_signal_connect(label_register, "activate-link", G_CALLBACK(on_register_clicked), NULL);
    g_signal_connect(label_register, "button-press-event", G_CALLBACK(on_register_clicked), NULL);

    gtk_widget_show_all(window);
}
