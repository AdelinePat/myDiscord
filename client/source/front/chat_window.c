#include "../../header/client_front.h"
#include "../../header/chat_window.h"
#include "../../header/login_window.h"
#include "../../header/register_window.h"
// #include <gtk/gtk.h>
#include <unistd.h> // pour send()
#include <string.h> // pour strlen()
#include "../../../controller/header/utils.h"

typedef struct
{
    GtkWidget *chat_display;
    GtkWidget *chat_entry;
} ChatWidgets;

// === Called when the "Disconnect" button is clicked ===
static void on_disconnect_clicked(GtkButton *button, gpointer user_data) {
    // GtkApplication *app = GTK_APPLICATION(user_data);
    Client_package_for_frontend *login_pack = (Client_package_for_frontend *)user_data;
    // GtkWidget **data = login_pack->data;
    GtkApplication *app = login_pack->app;
    // login_pack->data = NULL;
    GtkWidget **data = NULL;
    // gpointer data = NULL;
    // GtkApplication *app = GTK_APPLICATION(data);
    Login_infos *login_info = login_pack->client_package->login_info;

    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    if (GTK_IS_WINDOW(window))
    {
        gtk_widget_destroy(window); // Close the chat window
    }

    show_login_window(login_pack);
    // show_login_window(app, data, login_info);  // Return to login screen
    g_free(login_pack->data);
    free(login_pack);
}

// === Called when an emoji button is clicked inside the popover ===
static void on_emoji_clicked(GtkButton *button, gpointer user_data)
{
    GtkEntry *entry = GTK_ENTRY(user_data);
    // const gchar *emoji = gtk_button_get_label(GTK_WIDGET(button));
    const gchar *emoji = gtk_button_get_label(button);
    const gchar *current_text = gtk_entry_get_text(entry);

    gchar *new_text = g_strconcat(current_text, emoji, NULL);
    gtk_entry_set_text(entry, new_text);
    g_free(new_text);
}

// === Called when the user presses Enter in the chat entry ===
static void on_chat_entry_activate(GtkEntry *entry, gpointer user_data)
{
    Client_package_for_frontend *login_pack = (Client_package_for_frontend *)user_data;
    // ChatWidgets *widgets = (ChatWidgets *)user_data;

    const gchar *text = gtk_entry_get_text(entry);
    if (g_strcmp0(text, "") != 0)
    {
        send_message(login_pack->client_package, text);
        gtk_entry_set_text(entry, "");
    }
}

// === Called when the main emoji button is clicked to show popover ===
static void on_emoji_button_clicked(GtkButton *button, gpointer user_data)
{
    (void)button;
    gtk_popover_popup(GTK_POPOVER(user_data));
}

// === Called when the "Connect" button is clicked ===
static void on_connect_clicked(GtkButton *button, gpointer user_data)
{
    GtkApplication *app = GTK_APPLICATION(user_data);
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));

    // Récupérer les champs d’entrée
    GtkEntry *entry_user = g_object_get_data(G_OBJECT(window), "entry_user");
    GtkEntry *entry_pass = g_object_get_data(G_OBJECT(window), "entry_pass");

    const gchar *username = gtk_entry_get_text(entry_user);
    const gchar *password = gtk_entry_get_text(entry_pass);

    // Allouer la structure de connexion
    Client_package_for_frontend *login_pack = malloc(sizeof(Client_package_for_frontend));
    login_pack->client_package->login_info = malloc(sizeof(Login_infos));
    login_pack->client_package->client = malloc(sizeof(Client_data));
    login_pack->app = app;

    strncpy(login_pack->client_package->login_info->username,
        username,
        sizeof(login_pack->client_package->login_info->username) - 1);

    strncpy(login_pack->client_package->login_info->password,
        password,
        sizeof(login_pack->client_package->login_info->password) - 1);

    // SOCKET sock = client_start();
    // login_pack->client_package->client->sock_pointer = sock;

    // Tente la connexion
    if (login_attempts(login_pack) == 0)
    {
        // Authentification réussie
        recv(login_pack->client_package->client->sock_pointer,
            (char *)login_pack->client_package->client,
            sizeof(Client_data),
            0);

        // Lancer le thread de réception
        SOCKET *sock_ptr = malloc(sizeof(SOCKET));
        *sock_ptr = login_pack->client_package->client->sock_pointer;
        pthread_t recv_thread;
        pthread_create(&recv_thread, NULL, receive_messages, sock_ptr);
        pthread_detach(recv_thread);

        // Fermer la fenêtre de login
        gtk_widget_destroy(window);
        show_chat_window(login_pack);
    }
    else
    {
        // Échec
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
            "Échec de la connexion. Vérifiez vos identifiants.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        closesocket(login_pack->client_package->client->sock_pointer);

        free(login_pack->client_package->login_info);
        free(login_pack->client_package->client);
        free(login_pack);
    }
}

// === Displays the main chat window ===
void show_chat_window(Client_package_for_frontend *login_pack) {
    // Client_package_for_frontend *login_pack = login_pack;
    // GtkApplication **app = login_pack->app;

    GtkWidget *window, *outer_box, *chat_box, *chat_display, *chat_entry;
    GtkWidget *scrolled_window, *channels_box, *user_label, *bottom_box;
    GtkWidget *connect_button, *disconnect_button;

    window = gtk_application_window_new(login_pack->app);
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

    gchar *user_display_text = g_strdup_printf("User: %s", login_pack->client_package->client->client_name);
    user_label = gtk_label_new(user_display_text);
    g_free(user_display_text);                                              // Libérer la mémoire après utilisation
    gtk_widget_set_name(user_label, "user_label");                          // Définir un nom pour le widget
    gtk_box_pack_start(GTK_BOX(channels_box), user_label, FALSE, FALSE, 0); // Ajouter le label à la boîte

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
    for (int i = 0; i < 8; ++i)
    {
        GtkWidget *emoji_btn = gtk_button_new_with_label(emojis[i]);
        gtk_grid_attach(GTK_GRID(emoji_grid), emoji_btn, i % 4, i / 4, 1, 1);
        g_signal_connect(emoji_btn, "clicked", G_CALLBACK(on_emoji_clicked), chat_entry);
    }

    bottom_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(bottom_box, "bottom_box");
    gtk_box_pack_start(GTK_BOX(channels_box), bottom_box, FALSE, FALSE, 0);

    // CHAT DISPLAY DANS LOGIN_PACK
    login_pack->chat_display = chat_display;

    disconnect_button = gtk_button_new_with_label("Déconnexion");
    gtk_widget_set_name(disconnect_button, "disconnect_button");
    g_signal_connect(disconnect_button, "clicked", G_CALLBACK(on_disconnect_clicked), login_pack);
    gtk_box_pack_start(GTK_BOX(bottom_box), disconnect_button, FALSE, FALSE, 0);

    gtk_widget_show_all(emoji_grid);
    g_signal_connect(emoji_button, "clicked", G_CALLBACK(on_emoji_button_clicked), emoji_popover);

    ChatWidgets *chat_widgets = g_malloc(sizeof(ChatWidgets));
    if (chat_widgets == NULL)
    {
        g_warning("Failed to allocate memory for login_pack.");
        printf("Failed to allocate memory for login_pack."); // Or handle the error in an appropriate way
    }

    // chat_widgets->chat_display = chat_display;
    // chat_widgets->chat_entry = chat_entry;
    g_signal_connect(chat_entry, "activate", G_CALLBACK(on_chat_entry_activate), login_pack);

    GtkCssProvider *provider = gtk_css_provider_new();
    GError *error = NULL;
    gtk_css_provider_load_from_path(provider, "style.css", &error);
    if (error)
    {
        g_warning("CSS error: %s", error->message);
        g_clear_error(&error);
    }

    GtkWidget *widgets_to_style[] = {window, outer_box, user_label, chat_entry, chat_display, disconnect_button};
    for (int i = 0; i < G_N_ELEMENTS(widgets_to_style); ++i)
    {
        GtkStyleContext *context = gtk_widget_get_style_context(widgets_to_style[i]);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    }
    printf("dans chat_window juste avant recover_message");
    recover_messages(login_pack->client_package->client->sock_pointer, chat_display);
    gtk_widget_show_all(window);
    broadcast_notifications_receiver_start(login_pack);
}
