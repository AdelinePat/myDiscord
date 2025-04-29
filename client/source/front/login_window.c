#include <gtk/gtk.h>
#include <string.h>
#include "../../header/chat_window.h"
#include "../../header/register_window.h"

// Function to load the CSS file
void load_css(GtkApplication *app) {
    GtkSettings *default_settings = gtk_settings_get_default();
    g_object_set(G_OBJECT(default_settings),
                 "gtk-application-prefer-dark-theme", TRUE,
                 NULL);

    GtkCssProvider *css_provider = gtk_css_provider_new();
    GFile *css_file = g_file_new_for_path(STYLE_PATH);
    GError *error = NULL;

    if (!gtk_css_provider_load_from_file(css_provider, css_file, &error)) {
        g_warning("Failed to load CSS: %s", error->message);
        g_error_free(error);
    }

    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    g_object_unref(css_provider);
    g_object_unref(css_file);
}

// === CALLBACK: When the "Login" button is clicked ===
static void on_login_clicked(GtkButton *button, Login_package_for_front *login_pack) {
    GtkWidget **data = login_pack->data;
    GtkWidget *login_window = data[0];
    GtkWidget *entry_user = data[1];
    GtkWidget *entry_pass = data[2];
    GtkApplication *app = GTK_APPLICATION(data[3]);

    const gchar *user_name = gtk_entry_get_text(GTK_ENTRY(entry_user));
    const gchar *pass_word = gtk_entry_get_text(GTK_ENTRY(entry_pass));

    Login_infos *login_info = login_pack->login_info;
    strcpy(login_info->username, user_name);
    strcpy(login_info->password, pass_word);

    // Print login info to terminal
    printf("=== LOGIN INFO ===\n");
    printf("Username: %s\n", user_name);
    printf("Password: %s\n", pass_word);
    printf("Struct login_info.username = %s\n", login_info->username);
    printf("Struct login_info.password = %s\n", login_info->password);

    gtk_widget_destroy(login_window);
    show_chat_window(app, data, login_info);
}

// === CALLBACK: When the "Register" button is clicked ===
static void on_register_clicked(GtkButton *button, Login_package_for_front *login_pack) {
    GtkWidget **data = login_pack->data;
    GtkWidget *login_window = data[0];
    GtkApplication *app = GTK_APPLICATION(data[3]);

    g_print("Register link clicked!\n");
    gtk_widget_destroy(login_window);
    show_register_window(app, data, login_pack->login_info);
}

// === MAIN FUNCTION: Creates the login window ===
void show_login_window(GtkApplication *app, gpointer user_data, Login_infos *login_info) {
    load_css(app);

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Connexion");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 20);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_set_name(vbox, "main_background");

    GtkWidget *title_label = gtk_label_new("WHISPR");
    gtk_widget_set_name(title_label, "ui_title");
    gtk_widget_set_halign(title_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox), title_label, FALSE, FALSE, 0);

    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_name(separator, "separator_line");
    gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);

    GtkWidget *vbox_center = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_set_homogeneous(GTK_BOX(vbox_center), FALSE);

    GtkWidget *spacer_top = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox_center), spacer_top, TRUE, TRUE, 0);

    GtkWidget *label_user = gtk_label_new("Email/Username");
    gtk_widget_set_name(label_user, "entry_label");
    gtk_widget_set_halign(label_user, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox_center), label_user, FALSE, FALSE, 0);

    GtkWidget *entry_user = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_user), "Email or Username");
    gtk_widget_set_name(entry_user, "Placeholder");
    gtk_widget_set_size_request(entry_user, 500, -1);
    gtk_widget_set_halign(entry_user, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox_center), entry_user, FALSE, FALSE, 0);

    GtkWidget *spacer_middle = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox_center), spacer_middle, TRUE, TRUE, 0);

    GtkWidget *label_pass = gtk_label_new("Password");
    gtk_widget_set_name(label_pass, "entry_label");
    gtk_widget_set_halign(label_pass, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox_center), label_pass, FALSE, FALSE, 0);

    GtkWidget *entry_pass = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_pass), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(entry_pass), FALSE);
    gtk_widget_set_size_request(entry_pass, 500, -1);
    gtk_widget_set_halign(entry_pass, GTK_ALIGN_CENTER);
    gtk_widget_set_name(entry_pass, "Placeholder");
    gtk_box_pack_start(GTK_BOX(vbox_center), entry_pass, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(vbox), vbox_center, TRUE, TRUE, 0);

    GtkWidget *spacer = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox), spacer, TRUE, TRUE, 0);

    GtkWidget *hbox_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_buttons, FALSE, FALSE, 0);

    GtkWidget *btn_login = gtk_button_new_with_label("Login");
    GtkWidget *btn_register = gtk_button_new_with_label("Create Account");

    gtk_widget_set_name(btn_login, "left_button");
    gtk_widget_set_name(btn_register, "right_button");

    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_login, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_register, TRUE, TRUE, 5);

    Login_package_for_front *login_pack = g_new(Login_package_for_front, 1);
    login_pack->data = g_new(GtkWidget *, 4);
    login_pack->login_info = login_info;

    login_pack->data[0] = window;
    login_pack->data[1] = entry_user;
    login_pack->data[2] = entry_pass;
    login_pack->data[3] = GTK_WIDGET(app);

    g_signal_connect(btn_login, "clicked", G_CALLBACK(on_login_clicked), login_pack);
    g_signal_connect(btn_register, "clicked", G_CALLBACK(on_register_clicked), login_pack);

    gtk_widget_show_all(window);
}
