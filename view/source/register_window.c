#include <gtk/gtk.h>
#include "../header/login_window.h"

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
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 20);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_set_name(vbox, "main_background");

    GtkWidget *title_label = gtk_label_new("Création de Compte");
    gtk_widget_set_name(title_label, "create_title");
    gtk_widget_set_halign(title_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox), title_label, FALSE, FALSE, 0);

     // Créer une box pour les labels et les entrées avec un alignement vertical
    GtkWidget *vbox_center = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); // Cette box va contenir les labels et champs de saisie
    gtk_box_set_homogeneous(GTK_BOX(vbox_center), FALSE); // Les éléments peuvent avoir des tailles différentes
    
    // Ajoutez un espace au-dessus
    GtkWidget *spacer_top = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox_center), spacer_top, TRUE, TRUE, 0); // Cette box pousse les éléments vers le centre verticalement

    

    GtkWidget *label_username = gtk_label_new("Username");
    gtk_widget_set_name(label_username, "entry_label");
    gtk_widget_set_halign(label_username, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox_center), label_username, FALSE, FALSE, 0);
    
    GtkWidget *entry_user = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_user), "Username");
    gtk_widget_set_size_request(entry_user, 500, -1);
    gtk_widget_set_halign(entry_user, GTK_ALIGN_CENTER);


    
    gtk_widget_set_name(entry_user, "Password");
    gtk_box_pack_start(GTK_BOX(vbox_center), entry_user, FALSE, FALSE, 0);
    
   
    // Créer un autre espace (si nécessaire) entre les éléments
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
    gtk_widget_set_name(entry_email, "Password");
    gtk_box_pack_start(GTK_BOX(vbox_center), entry_email, FALSE, FALSE, 0);

    // Créer un autre espace (si nécessaire) entre les éléments
    GtkWidget *spacer_between_labels = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox_center), spacer_between_labels, TRUE, TRUE, 0);




    // Créer le label pour l'email
    GtkWidget *label_user = gtk_label_new("Password");
    gtk_widget_set_name(label_user, "entry_label");
    gtk_widget_set_halign(label_user, GTK_ALIGN_CENTER);  // Aligné au centre
    gtk_box_pack_start(GTK_BOX(vbox_center), label_user, FALSE, FALSE, 0);


    // Ajouter le vbox_center au vbox principal
    gtk_box_pack_start(GTK_BOX(vbox),vbox_center, TRUE, TRUE, 0); // vbox_center occupe la place centrale et l'alignera



    GtkWidget *entry_pass = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_pass), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(entry_pass), FALSE);
    gtk_widget_set_size_request(entry_pass, 500, -1);  //
    gtk_widget_set_halign(entry_pass, GTK_ALIGN_CENTER);
    gtk_widget_set_name(entry_pass, "Password");
    gtk_box_pack_start(GTK_BOX(vbox_center), entry_pass, FALSE, FALSE, 0);

    // Créer un autre espace (si nécessaire) entre les éléments
    GtkWidget *spacer_end_labels = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox_center), spacer_end_labels, TRUE, TRUE, 0);



    // Créer le label pour confirmer password
    GtkWidget *label_confirm_pass = gtk_label_new("Confirm Password");
    gtk_widget_set_name(label_confirm_pass, "entry_label");
    gtk_widget_set_halign(label_confirm_pass, GTK_ALIGN_CENTER);  // Aligné au centre
    gtk_box_pack_start(GTK_BOX(vbox_center), label_confirm_pass, FALSE, FALSE, 0);

    GtkWidget *entry_confirm_pass = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_confirm_pass), "Confirm Password");
    gtk_entry_set_visibility(GTK_ENTRY(entry_confirm_pass), FALSE);
    gtk_widget_set_size_request(entry_confirm_pass, 500, -1);
    gtk_widget_set_halign(entry_confirm_pass, GTK_ALIGN_CENTER);
    gtk_widget_set_name(entry_confirm_pass, "Password");
    gtk_box_pack_start(GTK_BOX(vbox_center), entry_confirm_pass, FALSE, FALSE, 0);

    // Ajouter le vbox_center au vbox principal
    gtk_box_pack_start(GTK_BOX(vbox),vbox_center, TRUE, TRUE, 0); // vbox_center occupe la place centrale et l'alignera

    // Spacer to push the buttons to the bottom
    GtkWidget *spacer = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox), spacer, TRUE, TRUE, 0);

    GtkWidget *hbox_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
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
