#include <gtk/gtk.h>

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Hello GTK");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);
    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
    printf("main() actually running! argc=%d\n", argc);

    GtkApplication *app;
    int status;

    app = gtk_application_new("org.example.myapp", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    printf("argc = %d\n", argc);
    for (int i = 0; i < argc; ++i) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    status = g_application_run(G_APPLICATION(app), argc, argv);

    // char *fake_argv[] = { argv[0], NULL };

    // status = g_application_run(G_APPLICATION(app), 1, fake_argv);

    g_object_unref(app);

    return status;
}
