#include <gtk/gtk.h>
#include "../header/login_window.h"
#include "../header/register_window.h"

typedef struct {
    GtkWidget *chat_display;
    GtkWidget *chat_entry;
} ChatWidgets;

// === Called when the "Disconnect" button is clicked ===
static void on_disconnect_clicked(GtkButton *button, gpointer user_data) {
    GtkApplication *app = GTK_APPLICATION(user_data);

    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    if (GTK_IS_WINDOW(window)) {
        gtk_widget_destroy(window);  // Close the chat window
    }

    show_login_window(app);  // Return to login screen
}

// === Called when an emoji button is clicked inside the popover ===
static void on_emoji_clicked(GtkButton *button, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(user_data);
    // const gchar *emoji = gtk_button_get_label(GTK_WIDGET(button));
    const gchar *emoji = gtk_button_get_label(button);
    const gchar *current_text = gtk_entry_get_text(entry);

    gchar *new_text = g_strconcat(current_text, emoji, NULL);
    gtk_entry_set_text(entry, new_text);
    g_free(new_text);
}

// === Called when the user presses Enter in the chat entry ===
static void on_chat_entry_activate(GtkEntry *entry, gpointer user_data) {
    ChatWidgets *widgets = (ChatWidgets *)user_data;

    const gchar *text = gtk_entry_get_text(entry);
    if (g_strcmp0(text, "") != 0) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->chat_display));
        GtkTextIter end_iter;
        gtk_text_buffer_get_end_iter(buffer, &end_iter);
        gtk_text_buffer_insert(buffer, &end_iter, text, -1);
        gtk_text_buffer_insert(buffer, &end_iter, "\n", -1);
        gtk_entry_set_text(entry, "");
    }
}

// === Called when the main emoji button is clicked to show popover ===
static void on_emoji_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    gtk_popover_popup(GTK_POPOVER(user_data));
}

// === Called when the "Connect" button is clicked ===
static void on_connect_clicked(GtkButton *button, gpointer user_data) {
    GtkApplication *app = GTK_APPLICATION(user_data);

    // TODO: Implement login verification logic here
}

// === Displays the main chat window ===
void show_chat_window(GtkApplication *app) {
    GtkWidget *window, *outer_box, *chat_box, *chat_display, *chat_entry;
    GtkWidget *scrolled_window, *channels_box, *user_label, *bottom_box;
    GtkWidget *connect_button, *disconnect_button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Whisper");
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);

    outer_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(outer_box, "main_background");
    gtk_container_add(GTK_CONTAINER(window), outer_box);

    channels_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(channels_box, "channels_box");
    gtk_widget_set_size_request(channels_box, 330, -1);
    gtk_box_pack_start(GTK_BOX(outer_box), channels_box, FALSE, FALSE, 0);

    GtkWidget *channel_label = gtk_label_new("Channel 1");
    gtk_box_pack_start(GTK_BOX(channels_box), channel_label, FALSE, FALSE, 0);

    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(spacer, TRUE);
    gtk_box_pack_start(GTK_BOX(channels_box), spacer, TRUE, TRUE, 0);

    user_label = gtk_label_new("User: Luffy");
    gtk_widget_set_name(user_label, "user_label");
    gtk_box_pack_start(GTK_BOX(channels_box), user_label, FALSE, FALSE, 0);

    chat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(chat_box, "chat_box");
    gtk_box_pack_start(GTK_BOX(outer_box), chat_box, TRUE, TRUE, 10);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_hexpand(scrolled_window, TRUE);
    gtk_widget_set_vexpand(scrolled_window, TRUE);

    chat_display = gtk_text_view_new();
    gtk_widget_set_name(chat_display, "chat_display");
    gtk_text_view_set_editable(GTK_TEXT_VIEW(chat_display), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(chat_display), GTK_WRAP_WORD_CHAR);
    gtk_container_add(GTK_CONTAINER(scrolled_window), chat_display);
    gtk_box_pack_start(GTK_BOX(chat_box), scrolled_window, TRUE, TRUE, 0);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(chat_display));
    GtkTextTag *default_tag = gtk_text_buffer_create_tag(buffer, "default", "foreground", "#D6D6D6", NULL);
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_apply_tag(buffer, default_tag, &start, &end);

    GtkWidget *input_line = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_end(GTK_BOX(chat_box), input_line, FALSE, FALSE, 0);

    chat_entry = gtk_entry_new();
    gtk_widget_set_name(chat_entry, "chat_entry");
    gtk_box_pack_end(GTK_BOX(input_line), chat_entry, TRUE, TRUE, 0);

    GtkWidget *emoji_button = gtk_button_new_with_label("😊");
    gtk_widget_set_name(emoji_button, "emoji_button");
    gtk_box_pack_end(GTK_BOX(input_line), emoji_button, FALSE, FALSE, 0);

    GtkWidget *emoji_popover = gtk_popover_new(emoji_button);
    GtkWidget *emoji_grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(emoji_popover), emoji_grid);

    const gchar *emojis[] = {"😊", "😂", "😍", "😎", "😉", "😭", "😡", "😃"};
    for (int i = 0; i < 8; ++i) {
        GtkWidget *emoji_btn = gtk_button_new_with_label(emojis[i]);
        gtk_grid_attach(GTK_GRID(emoji_grid), emoji_btn, i % 4, i / 4, 1, 1);
        g_signal_connect(emoji_btn, "clicked", G_CALLBACK(on_emoji_clicked), chat_entry);
    }

    bottom_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(bottom_box, "bottom_box");
    gtk_box_pack_start(GTK_BOX(channels_box), bottom_box, FALSE, FALSE, 0);

    disconnect_button = gtk_button_new_with_label("Déconnexion");
    gtk_widget_set_name(disconnect_button, "disconnect_button");
    g_signal_connect(disconnect_button, "clicked", G_CALLBACK(on_disconnect_clicked), app);
    gtk_box_pack_start(GTK_BOX(bottom_box), disconnect_button, FALSE, FALSE, 0);

    gtk_widget_show_all(emoji_grid);
    g_signal_connect(emoji_button, "clicked", G_CALLBACK(on_emoji_button_clicked), emoji_popover);

    ChatWidgets *chat_widgets = g_malloc(sizeof(ChatWidgets));
    chat_widgets->chat_display = chat_display;
    chat_widgets->chat_entry = chat_entry;
    g_signal_connect(chat_entry, "activate", G_CALLBACK(on_chat_entry_activate), chat_widgets);

    GtkCssProvider *provider = gtk_css_provider_new();
    GError *error = NULL;
    gtk_css_provider_load_from_path(provider, "style.css", &error);
    if (error) {
        g_warning("CSS error: %s", error->message);
        g_clear_error(&error);
    }

    GtkWidget *widgets_to_style[] = {window, outer_box, user_label, chat_entry, chat_display, disconnect_button};
    for (int i = 0; i < G_N_ELEMENTS(widgets_to_style); ++i) {
        GtkStyleContext *context = gtk_widget_get_style_context(widgets_to_style[i]);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    }

    gtk_widget_show_all(window);
}
