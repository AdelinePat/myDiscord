#include <gtk/gtk.h>
#include "../header/chat_window.h"
#include "../header/register_window.h"

// Function to load the CSS file
void load_css(GtkApplication *app) {
    // Create a new CssProvider to load the CSS
    GtkCssProvider *css_provider = gtk_css_provider_new();

    // Load the CSS file (replace "style.css" with the path to your CSS file)
    GFile *css_file = g_file_new_for_path(STYLE_PATH);
    GError *error = NULL;
    
    if (!gtk_css_provider_load_from_file(css_provider, css_file, &error)) {
        g_warning("Failed to load CSS: %s", error->message);
        g_error_free(error);
    }

    // Apply the CSS provider to the default screen
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(), 
        GTK_STYLE_PROVIDER(css_provider), 
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    // Clean up
    g_object_unref(css_provider);
    g_object_unref(css_file);
}

// === CALLBACK: When the "Login" button is clicked ===
static void on_login_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget **data = user_data;
    GtkWidget *login_window = data[0];
    GtkWidget *entry_user = data[1];
    GtkWidget *entry_pass = data[2];
    GtkApplication *app = GTK_APPLICATION(data[3]);

    const gchar *username = gtk_entry_get_text(GTK_ENTRY(entry_user));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(entry_pass));

    g_print("Attempting login: %s / %s\n", username, password);

    // TODO: Call the validate_credentials() function here to check the format of the email/username and password with regex
    //       Do not specify if it's the email or password that is incorrect
    // Example: if (!validate_credentials(username, password)) { show a generic error and return; }

    gtk_widget_destroy(login_window); // Closes the login window
    show_chat_window(app);           // Launches the chat window
}

// === CALLBACK: When the "Register" button is clicked ===
static void on_register_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget **data = user_data;
    GtkWidget *login_window = data[0];
    GtkApplication *app = GTK_APPLICATION(data[3]);

    g_print("Register link clicked!\n");

    // Close the login window before opening the registration window
    gtk_widget_destroy(login_window);

    // Opens the registration window
    show_register_window(app);
}

// === MAIN FUNCTION: Creates the login window ===
void show_login_window(GtkApplication *app) {
    load_css(app);  // Load and apply the CSS

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
    gtk_widget_set_name(separator, "separator_line");  // Give a custom ID to the separator line
    gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);

    // Create a box for labels and input fields with vertical alignment
    GtkWidget *vbox_center = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); // This box will contain the labels and input fields
    gtk_box_set_homogeneous(GTK_BOX(vbox_center), FALSE); // Elements can have different sizes

    // Add a spacer above
    GtkWidget *spacer_top = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox_center), spacer_top, TRUE, TRUE, 0); // This pushes elements to the center vertically

    // Create the label for email
    GtkWidget *label_user = gtk_label_new("Email/Username");
    gtk_widget_set_name(label_user, "entry_label");
    gtk_widget_set_halign(label_user, GTK_ALIGN_CENTER);  // Center aligned
    gtk_box_pack_start(GTK_BOX(vbox_center), label_user, FALSE, FALSE, 0);

    // Create the input field for email
    GtkWidget *entry_user = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_user), "Email or Username");
    gtk_widget_set_name(entry_user, "Placeholder");
    gtk_widget_set_size_request(entry_user, 500, -1);  // Limit width to 500px
    gtk_widget_set_halign(entry_user, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox_center), entry_user, FALSE, FALSE, 0);

    // Create another spacer (if needed) between elements
    GtkWidget *spacer_middle = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox_center), spacer_middle, TRUE, TRUE, 0);

    // Create the label for password
    GtkWidget *label_pass = gtk_label_new("Password");
    gtk_widget_set_name(label_pass, "entry_label");
    gtk_widget_set_halign(label_pass, GTK_ALIGN_CENTER);  // Center aligned
    gtk_box_pack_start(GTK_BOX(vbox_center), label_pass, FALSE, FALSE, 0);

    // Create the input field for password
    GtkWidget *entry_pass = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_pass), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(entry_pass), FALSE);
    gtk_widget_set_size_request(entry_pass, 500, -1);  // Limit width to 500px
    gtk_widget_set_halign(entry_pass, GTK_ALIGN_CENTER);
    gtk_widget_set_name(entry_pass, "Placeholder");
    gtk_box_pack_start(GTK_BOX(vbox_center), entry_pass, FALSE, FALSE, 0);

    // Add the vbox_center to the main vbox
    gtk_box_pack_start(GTK_BOX(vbox), vbox_center, TRUE, TRUE, 0); // vbox_center takes the central space and aligns it

    // Space to push buttons to the bottom
    GtkWidget *spacer = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox), spacer, TRUE, TRUE, 0);

    // Create a horizontal box for the buttons
    GtkWidget *hbox_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_buttons, FALSE, FALSE, 0);

    // Create the buttons
    GtkWidget *btn_login = gtk_button_new_with_label("Login");
    GtkWidget *btn_register = gtk_button_new_with_label("Create Account");

    // Set the CSS classes to apply styles from your stylesheet
    gtk_widget_set_name(btn_login, "left_button"); // Assign CSS ID for button styling
    gtk_widget_set_name(btn_register, "right_button"); // Assign CSS ID for button styling

    // Add buttons to the hbox
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_login, TRUE, TRUE, 5); 
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_register, TRUE, TRUE, 5);

    // Allocate data to pass to the signal function
    GtkWidget **data = g_new(GtkWidget *, 4);
    data[0] = window;
    data[1] = entry_user;
    data[2] = entry_pass;
    data[3] = GTK_WIDGET(app);

    // Connect signals for login and register buttons
    g_signal_connect(btn_login, "clicked", G_CALLBACK(on_login_clicked), data);
    g_signal_connect(btn_register, "clicked", G_CALLBACK(on_register_clicked), data); // Call on_register_clicked

    gtk_widget_show_all(window);
}
