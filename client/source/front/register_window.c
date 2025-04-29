#include <gtk/gtk.h>
#include "../../header/login_window.h"

// === CALLBACK: When the "Confirmer" button is clicked ===
static void on_confirm_clicked(GtkButton *button, Login_package_for_front *login_pack) {
    (void)button;

    GtkWidget **data = login_pack->data;
    // GtkWidget **data = user_data;
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

    Login_infos *login_info = login_pack->login_info;

    strcpy(login_info->username, username);
    strcpy(login_info->email, email);
    strcpy(login_info->password, password);
    strcpy(login_info->confirm_password, confirm_password);
    login_info->login_register = 1; // login_register = NULL or 0 if login and 1 or anything if first time register

    if (g_strcmp0(password, confirm_password) != 0) {
        g_print("Passwords do not match!\n");
        return;
    }

    g_print("Registration with: %s / %s / %s\n", username, email, password);
    g_print("Registration with (struct value): %s / %s / %s\n", login_info->username, login_info->email, login_info->password);
    gtk_widget_destroy(register_window);
    show_login_window(app, data, login_info);
}

// === CALLBACK: When the "Retour" button is clicked ===
static void on_return_clicked(GtkButton *button, Login_package_for_front *login_pack) {
    (void)button;
    GtkWidget **data = login_pack->data;
    // GtkWidget **data = user_data;
    GtkWidget *register_window = data[0];
    GtkApplication *app = GTK_APPLICATION(data[5]);

    Login_infos *login_info = login_pack->login_info;
    gtk_widget_destroy(register_window);
    show_login_window(app, data, login_info);
}

// === MAIN FUNCTION: Creates the registration window ===
void show_register_window(GtkApplication *app, Login_infos *login_info) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Create Account");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 20);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_set_name(vbox, "main_background");

    GtkWidget *title_label = gtk_label_new("Création de Compte");
    gtk_widget_set_name(title_label, "ui_title");
    gtk_widget_set_halign(title_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox), title_label, FALSE, FALSE, 0);

    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_name(separator, "separator_line");  // Give a custom ID to the separator line
    gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);

    // Create a box for labels and entries with vertical alignment
    GtkWidget *vbox_center = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); // This box will contain the labels and entry fields
    gtk_box_set_homogeneous(GTK_BOX(vbox_center), FALSE); // Elements can have different sizes
    
    // Add space above
    GtkWidget *spacer_top = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox_center), spacer_top, TRUE, TRUE, 0); // This pushes the elements to the vertical center

    GtkWidget *label_username = gtk_label_new("Username");
    gtk_widget_set_name(label_username, "entry_label");
    gtk_widget_set_halign(label_username, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox_center), label_username, FALSE, FALSE, 0);
    
    GtkWidget *entry_user = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_user), "Username");
    gtk_widget_set_size_request(entry_user, 500, -1);
    gtk_widget_set_halign(entry_user, GTK_ALIGN_CENTER);
    gtk_widget_set_name(entry_user, "Placeholder");
    gtk_box_pack_start(GTK_BOX(vbox_center), entry_user, FALSE, FALSE, 0);
    
    // Create another space (if necessary) between elements
    GtkWidget *spacer_middle = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox_center), spacer_middle, TRUE, TRUE, 0);

    GtkWidget *label_email = gtk_label_new("Email");
    gtk_widget_set_name(label_email, "entry_label");
    gtk_widget_set_halign(label_email, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox_center), label_email, FALSE, FALSE, 0);
    
    GtkWidget *entry_email = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_email), "Email");
    gtk_widget_set_size_request(entry_email, 500, -1);
    gtk_widget_set_halign(entry_email, GTK_ALIGN_CENTER);
    gtk_widget_set_name(entry_email, "Placeholder");
    gtk_box_pack_start(GTK_BOX(vbox_center), entry_email, FALSE, FALSE, 0);

    // Create another space (if necessary) between elements
    GtkWidget *spacer_between_labels = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox_center), spacer_between_labels, TRUE, TRUE, 0);

    // Create the label for password
    GtkWidget *label_user = gtk_label_new("Password");
    gtk_widget_set_name(label_user, "entry_label");
    gtk_widget_set_halign(label_user, GTK_ALIGN_CENTER);  // Center aligned
    gtk_box_pack_start(GTK_BOX(vbox_center), label_user, FALSE, FALSE, 0);

    // Add vbox_center to the main vbox
    gtk_box_pack_start(GTK_BOX(vbox),vbox_center, TRUE, TRUE, 0); // vbox_center will take the central space and be aligned

    GtkWidget *entry_pass = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_pass), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(entry_pass), FALSE);
    gtk_widget_set_size_request(entry_pass, 500, -1);
    gtk_widget_set_halign(entry_pass, GTK_ALIGN_CENTER);
    gtk_widget_set_name(entry_pass, "Placeholder");
    gtk_box_pack_start(GTK_BOX(vbox_center), entry_pass, FALSE, FALSE, 0);

    // Create another space (if necessary) between elements
    GtkWidget *spacer_end_labels = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox_center), spacer_end_labels, TRUE, TRUE, 0);

    // Create the label for confirm password
    GtkWidget *label_confirm_pass = gtk_label_new("Confirm Password");
    gtk_widget_set_name(label_confirm_pass, "entry_label");
    gtk_widget_set_halign(label_confirm_pass, GTK_ALIGN_CENTER);  // Center aligned
    gtk_box_pack_start(GTK_BOX(vbox_center), label_confirm_pass, FALSE, FALSE, 0);

    GtkWidget *entry_confirm_pass = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_confirm_pass), "Confirm Password");
    gtk_entry_set_visibility(GTK_ENTRY(entry_confirm_pass), FALSE);
    gtk_widget_set_size_request(entry_confirm_pass, 500, -1);
    gtk_widget_set_halign(entry_confirm_pass, GTK_ALIGN_CENTER);
    gtk_widget_set_name(entry_confirm_pass, "Placeholder");
    gtk_box_pack_start(GTK_BOX(vbox_center), entry_confirm_pass, FALSE, FALSE, 0);

    // Add vbox_center to the main vbox
    gtk_box_pack_start(GTK_BOX(vbox),vbox_center, TRUE, TRUE, 0); // vbox_center will take the central space and be aligned

    // Spacer to push the buttons to the bottom
    GtkWidget *spacer = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox), spacer, TRUE, TRUE, 0);

    GtkWidget *hbox_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_buttons, FALSE, FALSE, 0);

    GtkWidget *btn_confirm = gtk_button_new_with_label("Confirmer");
    GtkWidget *btn_return = gtk_button_new_with_label("Retour");

    gtk_widget_set_name(btn_confirm, "left_button");
    gtk_widget_set_name(btn_return, "right_button");

    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_confirm, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_return, TRUE, TRUE, 5);

    Login_package_for_front *login_pack;
    login_pack->data = g_new(GtkWidget *, 6);
    login_pack->login_info = login_info;

    // login_pack->data[0] = window;
    // login_pack->data[1] = entry_user;
    // login_pack->data[2] = entry_pass;
    // login_pack->data[3] = GTK_WIDGET(app);

    // GtkWidget **data = g_new(GtkWidget *, 6);
    login_pack->data[0] = window;
    login_pack->data[1] = entry_user;
    login_pack->data[2] = entry_email;
    login_pack->data[3] = entry_pass;
    login_pack->data[4] = entry_confirm_pass;
    login_pack->data[5] = GTK_WIDGET(app);

    g_signal_connect(btn_confirm, "clicked", G_CALLBACK(on_confirm_clicked), login_pack);
    g_signal_connect(btn_return, "clicked", G_CALLBACK(on_return_clicked), login_pack);

    gtk_widget_show_all(window);
}
