#include <gtk/gtk.h>

static void on_toggle_clicked(GtkToggleButton *toggle_button, gpointer user_data) {
    GtkLabel *label = GTK_LABEL(user_data);
    if (gtk_toggle_button_get_active(toggle_button)) {
        gtk_label_set_markup(label, "<span font_desc=\"Arial Bold 24\" foreground=\"green\">Discord_blablaconnecté !!!</span>");
    } else {
        gtk_label_set_markup(label, "<span font_desc=\"Arial Bold 24\" foreground=\"blue\">Discord_blabla_disconnected</span>");
    }
}

static void on_connect_clicked(GtkButton *button, gpointer user_data) {
    g_print("Connexion bouton cliqué !\n");
}

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *outer_box;
    GtkWidget *inner_box;
    GtkWidget *bottom_box;
    GtkWidget *label;
    GtkWidget *toggle_button;
    GtkWidget *connect_button;
    GtkWidget *user_label;

    // Fenêtre principale
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Whisper");
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);

    // Boîte verticale principale
    outer_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(outer_box, "main_background");
    gtk_widget_set_hexpand(outer_box, TRUE);
    gtk_widget_set_vexpand(outer_box, TRUE);
    gtk_container_add(GTK_CONTAINER(window), outer_box);

    // Boîte intérieure centrée
    inner_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(inner_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(inner_box, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(outer_box), inner_box, TRUE, TRUE, 0);

    // Label principal
    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label),
        "<span font_desc=\"Arial Bold 24\" foreground=\"red\">Hello discord Blabla</span>");
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(inner_box), label, TRUE, TRUE, 0);

    // Toggle button
    toggle_button = gtk_toggle_button_new_with_label("Click");
    gtk_widget_set_name(toggle_button, "my_button");
    g_signal_connect(toggle_button, "toggled", G_CALLBACK(on_toggle_clicked), label);
    gtk_widget_set_halign(toggle_button, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(inner_box), toggle_button, TRUE, TRUE, 0);

    // Boîte du bas : bouton gauche + label utilisateur à droite
    bottom_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_widget_set_halign(bottom_box, GTK_ALIGN_FILL);
    gtk_widget_set_valign(bottom_box, GTK_ALIGN_END);
    gtk_box_pack_start(GTK_BOX(outer_box), bottom_box, FALSE, TRUE, 10);

    // Bouton connexion
    connect_button = gtk_button_new_with_label("Connexion");
    gtk_widget_set_name(connect_button, "connect_button");
    g_signal_connect(connect_button, "clicked", G_CALLBACK(on_connect_clicked), NULL);
    gtk_widget_set_halign(connect_button, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(bottom_box), connect_button, FALSE, FALSE, 20);

    // Label utilisateur
    user_label = gtk_label_new("Utilisateur: Enola");
    gtk_widget_set_name(user_label, "user_label");
    gtk_widget_set_halign(user_label, GTK_ALIGN_END);
    gtk_box_pack_end(GTK_BOX(bottom_box), user_label, FALSE, FALSE, 20);

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
        window, outer_box, toggle_button, connect_button, user_label
    };

    for (int i = 0; i < sizeof(widgets_to_style)/sizeof(GtkWidget*); ++i) {
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
