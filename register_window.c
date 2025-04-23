#include <gtk/gtk.h>
#include "login_window.h"

// === CALLBACK: When the "Confirmer" button is clicked ===
static void on_confirm_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GtkWidget **data = user_data;
    GtkWidget *register_window = data[0];
    GtkWidget *entry_user = data[1];
    GtkWidget *entry_email = data[2];
    GtkWidget *entry_pass = data[3];
    GtkWidget *entry_confirm_pass = data[4];
    GtkApplication *app = GTK_APPLICATION(data[5]);

    const gchar *username = gtk_entry_get_text(GTK_ENTRY(entry_user));
    const gchar *email = gtk_entry_get_text(GTK_ENTRY(entry_email));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(entry_pass));
    const gchar *confirm_password = gtk_entry_get_text(GTK_ENTRY(entry_confirm_pass));

    if (g_strcmp0(password, confirm_password) != 0) {
        g_print("Passwords do not match!\n");
        return;
    }

    g_print("Registration with: %s / %s / %s\n", username, email, password);
    gtk_widget_destroy(register_window);
    show_login_window(app);
}

// === CALLBACK: When the "Retour" button is clicked ===
static void on_return_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    GtkWidget **data = user_data;
    GtkWidget *register_window = data[0];
    GtkApplication *app = GTK_APPLICATION(data[5]);

    gtk_widget_destroy(register_window);
    show_login_window(app);
}

// === MAIN FUNCTION: Creates the registration window ===
void show_register_window(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Create Account");
    gtk_window_set_default_size(GTK_WINDOW(window), 720, 450);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 20);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_set_name(vbox, "main_background");

    GtkWidget *entry_user = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_user), "Username");
    gtk_box_pack_start(GTK_BOX(vbox), entry_user, FALSE, FALSE, 0);

    GtkWidget *entry_email = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_email), "Email");
    gtk_box_pack_start(GTK_BOX(vbox), entry_email, FALSE, FALSE, 0);

    GtkWidget *entry_pass = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_pass), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(entry_pass), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), entry_pass, FALSE, FALSE, 0);

    GtkWidget *entry_confirm_pass = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_confirm_pass), "Confirm Password");
    gtk_entry_set_visibility(GTK_ENTRY(entry_confirm_pass), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), entry_confirm_pass, FALSE, FALSE, 0);

    // Spacer to push the buttons to the bottom
    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(vbox), spacer, TRUE, TRUE, 0);

    GtkWidget *hbox_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(hbox_buttons, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(hbox_buttons, GTK_ALIGN_END);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_buttons, FALSE, FALSE, 0);

    GtkWidget *btn_confirm = gtk_button_new_with_label("Confirmer");
    GtkWidget *btn_return = gtk_button_new_with_label("Retour");

    gtk_widget_set_name(btn_confirm, "confirm_button");
    gtk_widget_set_name(btn_return, "return_button");

    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_confirm, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_return, TRUE, TRUE, 5);

    GtkWidget **data = g_new(GtkWidget *, 6);
    data[0] = window;
    data[1] = entry_user;
    data[2] = entry_email;
    data[3] = entry_pass;
    data[4] = entry_confirm_pass;
    data[5] = GTK_WIDGET(app);

    g_signal_connect(btn_confirm, "clicked", G_CALLBACK(on_confirm_clicked), data);
    g_signal_connect(btn_return, "clicked", G_CALLBACK(on_return_clicked), data);

    gtk_widget_show_all(window);
}
