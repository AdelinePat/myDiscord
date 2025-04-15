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
    GtkWidget *label;
    GtkWidget *box;
    GtkWidget *toggle_button;

    // Create a new top-level window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "My_discord_blabla");
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);

    // Set minimum window size
    GdkGeometry hints;
    hints.min_width = 500;
    hints.min_height = 300;
    gtk_window_set_geometry_hints(GTK_WINDOW(window), NULL, &hints, GDK_HINT_MIN_SIZE);

    // Create a vertical box container
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Create a label with custom font, size, and color
    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label),
        "<span font_desc=\"Arial Bold 24\" foreground=\"blue\">Hello World</span>");
    
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER); // horizontal alignment
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);

    // Create a toggle button
    toggle_button = gtk_toggle_button_new_with_label("Appuyer");
    g_signal_connect(toggle_button, "toggled", G_CALLBACK(on_toggle_clicked), label);

    // Change font size for the toggle button label
    PangoFontDescription *font_desc = pango_font_description_from_string("Arial Bold 18");
    gtk_widget_override_font(toggle_button, font_desc);
    pango_font_description_free(font_desc); // Free font description after use

    gtk_box_pack_start(GTK_BOX(box), toggle_button, FALSE, FALSE, 0);

    // Show everything
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
