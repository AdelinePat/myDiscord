#include <gtk/gtk.h>

typedef struct {
    GtkWidget *chat_display;
    GtkWidget *chat_entry;
} ChatWidgets;

// Called when the "Connect" button is clicked
static void on_connect_clicked(GtkButton *button, gpointer user_data) {
    g_print("Connect button clicked!\n");
}

// Called when the user presses "Enter" in the GtkEntry field
static void on_chat_entry_activate(GtkEntry *entry, gpointer user_data) {
    ChatWidgets *widgets = (ChatWidgets *)user_data; // Cast user_data to ChatWidgets

    const gchar *text = gtk_entry_get_text(entry); // Get text from entry

    if (g_strcmp0(text, "") != 0) { // Check if the entry is not empty
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->chat_display)); // Get text buffer
        GtkTextIter end_iter;
        gtk_text_buffer_get_end_iter(buffer, &end_iter); // Get iterator at the end of buffer

        gtk_text_buffer_insert(buffer, &end_iter, text, -1); // Insert text at the end
        gtk_text_buffer_insert(buffer, &end_iter, "\n", -1); // Add a newline
        gtk_entry_set_text(GTK_ENTRY(entry), ""); // Clear entry field
    }
}
static void on_emoji_clicked(GtkButton *button, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(user_data);
    const gchar *current_text = gtk_entry_get_text(entry);
    const gchar *emoji = gtk_button_get_label(GTK_WIDGET(button)); // "😊"

    gchar *new_text = g_strconcat(current_text, emoji, NULL);
    gtk_entry_set_text(entry, new_text);
    g_free(new_text);
}


static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *outer_box;
    GtkWidget *chat_box;
    GtkWidget *chat_display;
    GtkWidget *chat_entry;
    GtkWidget *scrolled_window;
    GtkWidget *channels_box;
    GtkWidget *connect_button;
    GtkWidget *user_label;

    // Main window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Whisper_blabla");
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);

    // Horizontal main box for layout: channels on the left, chat on the right
    outer_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(outer_box, "main_background");
    gtk_widget_set_hexpand(outer_box, TRUE);
    gtk_widget_set_vexpand(outer_box, TRUE);
    gtk_container_add(GTK_CONTAINER(window), outer_box);

    // Channels box (left side)
    channels_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(channels_box, "channels_box");
    gtk_widget_set_vexpand(channels_box, TRUE);
    gtk_widget_set_size_request(channels_box, 330, -1); // Fixed width
    gtk_box_pack_start(GTK_BOX(outer_box), channels_box, FALSE, FALSE, 0);

    // Example channel
    GtkWidget *channel_label = gtk_label_new("Channel 1");
    gtk_box_pack_start(GTK_BOX(channels_box), channel_label, FALSE, FALSE, 0);

    // Spacer widget to push content to bottom
    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(spacer, TRUE); // This makes it take all vertical space
    gtk_box_pack_start(GTK_BOX(channels_box), spacer, TRUE, TRUE, 0);

    // Bottom box to hold the connect button and user label
    GtkWidget *bottom_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(bottom_box, "bottom_box");
    gtk_box_pack_start(GTK_BOX(channels_box), bottom_box, FALSE, FALSE, 0);

    // Connect button
    connect_button = gtk_button_new_with_label("Connexion");
    gtk_widget_set_name(connect_button, "connect_button");
    g_signal_connect(connect_button, "clicked", G_CALLBACK(on_connect_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(bottom_box), connect_button, FALSE, FALSE, 0);

    // User label
    user_label = gtk_label_new("User: Shadows");
    gtk_widget_set_name(user_label, "user_label");
    gtk_box_pack_start(GTK_BOX(bottom_box), user_label, FALSE, FALSE, 0);


    // ======= Chat area =======
    chat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(chat_box, "chat_box");
    gtk_widget_set_hexpand(chat_box, TRUE);
    gtk_widget_set_vexpand(chat_box, TRUE);
    gtk_box_pack_start(GTK_BOX(outer_box), chat_box, TRUE, TRUE, 10);

    // Scrollable chat window
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_hexpand(scrolled_window, TRUE);
    gtk_widget_set_vexpand(scrolled_window, TRUE);

    // Chat display (GtkTextView)
    chat_display = gtk_text_view_new();
    gtk_widget_set_name(chat_display, "chat_display");
    gtk_text_view_set_editable(GTK_TEXT_VIEW(chat_display), FALSE);
    gtk_widget_set_hexpand(chat_display, TRUE);
    gtk_widget_set_size_request(chat_display, 980, -1); // Limit width
    gtk_widget_set_vexpand(chat_display, TRUE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(chat_display), GTK_WRAP_WORD_CHAR); // Enable word wrap

    // Add chat_display to the scrolled window
    gtk_container_add(GTK_CONTAINER(scrolled_window), chat_display);
    gtk_box_pack_start(GTK_BOX(chat_box), scrolled_window, TRUE, TRUE, 0);

    // Apply default text style (color)
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(chat_display));
    GtkTextTag *default_tag = gtk_text_buffer_create_tag(buffer, "default",
        "foreground", "#D6D6D6", NULL);
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_apply_tag(buffer, default_tag, &start, &end);

    // Bottom input line with text field
    GtkWidget *input_line = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_hexpand(input_line, TRUE);
    gtk_box_pack_end(GTK_BOX(chat_box), input_line, FALSE, FALSE, 0);

    // Text entry field
    chat_entry = gtk_entry_new();
    gtk_widget_set_name(chat_entry, "chat_entry");
    gtk_widget_set_hexpand(chat_entry, TRUE);
    gtk_box_pack_end(GTK_BOX(input_line), chat_entry, TRUE, TRUE, 0);

    // Emoji button
    GtkWidget *emoji_button = gtk_button_new_with_label("😊");
    gtk_widget_set_name(emoji_button, "emoji_button");
    gtk_box_pack_end(GTK_BOX(input_line), emoji_button, FALSE, FALSE, 0);

    g_signal_connect(emoji_button, "clicked", G_CALLBACK(on_emoji_clicked), chat_entry);



    // Store references in struct
    ChatWidgets *chat_widgets = g_malloc(sizeof(ChatWidgets));
    chat_widgets->chat_display = chat_display;
    chat_widgets->chat_entry = chat_entry;

    g_signal_connect(chat_entry, "activate", G_CALLBACK(on_chat_entry_activate), chat_widgets);

    // Load CSS style
    GtkCssProvider *provider = gtk_css_provider_new();
    GError *error = NULL;
    gtk_css_provider_load_from_path(provider, "style.css", &error);
    if (error) {
        g_warning("CSS error: %s", error->message);
        g_clear_error(&error);
    }

    // Apply CSS to widgets
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
