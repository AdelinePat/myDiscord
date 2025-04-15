// GTK is a multi-platform toolkit for creating graphical user interfaces.
// This header includes all the necessary GTK+ 3 functions, widgets, and macros
// needed to build GUI applications in C.
#include <gtk/gtk.h>

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "My_discord_blabla2");

    // Default window size
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);

    // Set minimum window size (prevents resizing below this size)
    GdkGeometry hints;
    hints.min_width = 500;
    hints.min_height = 300;
    gtk_window_set_geometry_hints(GTK_WINDOW(window), NULL, &hints, GDK_HINT_MIN_SIZE);

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
