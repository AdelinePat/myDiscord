#include <gtk/gtk.h>

static void on_connect_clicked(GtkButton *button, gpointer user_data) {
    g_print("Connexion bouton cliqué !\n");
}

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *outer_box;
    GtkWidget *inner_box;
    GtkWidget *label;
    GtkWidget *connect_button;
    GtkWidget *user_label;
    GtkWidget *chat_box;
    GtkWidget *chat_display;
    GtkWidget *chat_entry;

    // Fenêtre principale
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Whisper_blabla");
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);

    // Boîte verticale principale
    outer_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(outer_box, "main_background");
    gtk_widget_set_hexpand(outer_box, TRUE);
    gtk_widget_set_vexpand(outer_box, TRUE);
    gtk_container_add(GTK_CONTAINER(window), outer_box);

    // Boîte intérieure pour le label, alignée en haut à gauche
    inner_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(inner_box, GTK_ALIGN_START);
    gtk_widget_set_valign(inner_box, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(outer_box), inner_box, FALSE, FALSE, 0);

    // Label principal
    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span font_desc='Arial 24' color='#D6D6D6'>Channel</span>");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(inner_box), label, TRUE, TRUE, 0);

    // ======= Chat zone =======
    chat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(chat_box, "chat_box");
    gtk_widget_set_hexpand(chat_box, TRUE);
    gtk_widget_set_vexpand(chat_box, TRUE);
    gtk_box_pack_start(GTK_BOX(outer_box), chat_box, TRUE, TRUE, 10);

    // Chat display
    chat_display = gtk_text_view_new();
    gtk_widget_set_name(chat_display, "chat_display");
    gtk_text_view_set_editable(GTK_TEXT_VIEW(chat_display), FALSE);
    gtk_widget_set_hexpand(chat_display, TRUE);
    gtk_widget_set_size_request(chat_display, 950, -1);
    gtk_widget_set_hexpand(chat_display, FALSE); 
    gtk_widget_set_halign(chat_display, GTK_ALIGN_END); 
    gtk_widget_set_vexpand(chat_display, TRUE);// Largeur = 600px, hauteur auto
    gtk_box_pack_start(GTK_BOX(chat_box), chat_display, TRUE, TRUE, 0);

    

    // Forcer le style du texte si nécessaire
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(chat_display));
    GtkTextTag *default_tag = gtk_text_buffer_create_tag(buffer, "default",
        "foreground", "#D6D6D6", NULL);
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_apply_tag(buffer, default_tag, &start, &end);

    // Ligne du bas avec les 3 éléments
    GtkWidget *input_line = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_hexpand(input_line, TRUE);
    gtk_box_pack_end(GTK_BOX(chat_box), input_line, FALSE, FALSE, 0);

    // Bouton connexion
    connect_button = gtk_button_new_with_label("Connexion");
    gtk_widget_set_name(connect_button, "connect_button");
    g_signal_connect(connect_button, "clicked", G_CALLBACK(on_connect_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(input_line), connect_button, FALSE, FALSE, 0);

    // Label utilisateur
    user_label = gtk_label_new("Utilisateur: Enola");
    gtk_widget_set_name(user_label, "user_label");
    gtk_box_pack_start(GTK_BOX(input_line), user_label, FALSE, FALSE, 0);

    // Champ de saisie
    chat_entry = gtk_entry_new();
    gtk_widget_set_name(chat_entry, "chat_entry");
    gtk_widget_set_hexpand(chat_entry, TRUE);
    gtk_box_pack_end(GTK_BOX(input_line), chat_entry, TRUE, TRUE, 0);

    // Chargement CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    GError *error = NULL;
    gtk_css_provider_load_from_path(provider, "style.css", &error);
    if (error) {
        g_warning("Erreur CSS : %s", error->message);
        g_clear_error(&error);
    }

    // Application du style
    GtkStyleContext *context;
    GtkWidget *widgets_to_style[] = {
        window, outer_box, connect_button, user_label, chat_entry, chat_display
    };

    for (int i = 0; i < sizeof(widgets_to_style) / sizeof(GtkWidget*); ++i) {
        context = gtk_widget_get_style_context(widgets_to_style[i]);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    }

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.example.myapp", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
