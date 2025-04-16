#include <gtk/gtk.h>

static void on_toggle_clicked(GtkToggleButton *toggle_button, gpointer user_data) {
    GtkLabel *label = GTK_LABEL(user_data);
    if (gtk_toggle_button_get_active(toggle_button)) {
        gtk_label_set_markup(label, "<span font_desc=\"Arial Bold 24\" foreground=\"green\">Discord_blablaconnecté !!!</span>");
    } else {
        gtk_label_set_markup(label, "<span font_desc=\"Arial Bold 24\" foreground=\"blue\">Hello World</span>");
    }
}

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *outer_box;
    GtkWidget *inner_box;
    GtkWidget *label;
    GtkWidget *toggle_button;

    // Fenêtre principale
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "My_discord_blabla5");
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);

    // Boîte extérieure pour le fond
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

    // Label
    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label),
        "<span font_desc=\"Arial Bold 24\" foreground=\"red\">Hello World</span>");
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(inner_box), label, TRUE, TRUE, 0);

    // Toggle button
    toggle_button = gtk_toggle_button_new_with_label("Appuyer");
    gtk_widget_set_name(toggle_button, "my_button");
    g_signal_connect(toggle_button, "toggled", G_CALLBACK(on_toggle_clicked), label);
    gtk_widget_set_halign(toggle_button, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(inner_box), toggle_button, TRUE, TRUE, 0);

    // CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "#main_background {"
        "  background-color: #007FFF;"  // Bleu clair
        "}"
        "#my_button {"
        "  background-color: #003366;"
        "  color: black;"
        "  font: bold 18pt 'Arial';"
        "  border-radius: 5px;"
        "  padding: 10px 20px;"
        "}"
        "#my_button:hover {"
        "  background-color: red;"
        "}"
        "#my_button:checked {"
        "  background-color: green;"
        "  color: black;"
        "}"
        "#my_button:active {"
        "  background-color: #005599;"
        "}",
    -1, NULL);

    // Appliquer style
    GtkStyleContext *context;

    context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(outer_box);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(toggle_button);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

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
