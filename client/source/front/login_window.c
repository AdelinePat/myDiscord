// #include <gtk/gtk.h>
#include "../../header/client_front.h"
#include "../../header/login_window.h"
#include "../../header/chat_window.h"
#include "../../header/register_window.h"
#include "../../header/connector.h"
#include "../../../controller/header/utils.h"
#include "../../header/hash_login.h"
#include "../../header/session.h"
#include <libpq-fe.h>
#include <stdlib.h>

// Function to load the CSS file
void load_css(GtkApplication *app)
{

    // change default settings from bright theme to dark theme
    GtkSettings *default_settings = gtk_settings_get_default();
    g_object_set(G_OBJECT(default_settings),
                 "gtk-application-prefer-dark-theme", TRUE,
                 NULL);

    // Create a new CssProvider to load the CSS
    GtkCssProvider *css_provider = gtk_css_provider_new();

    // Load the CSS file (replace "style.css" with the path to your CSS file)
    GFile *css_file = g_file_new_for_path(STYLE_PATH);
    GError *error = NULL;

    if (!gtk_css_provider_load_from_file(css_provider, css_file, &error))
    {
        g_warning("Failed to load CSS: %s", error->message);
        g_error_free(error);
    }

    // Apply the CSS provider to the default screen
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Clean up
    g_object_unref(css_provider);
    g_object_unref(css_file);
}

// === CALLBACK: When the "Login" button is clicked ===

static void on_login_clicked(GtkButton *button, gpointer user_data)
{
    Login_package_for_front *login_pack = (Login_package_for_front *)user_data;

    GtkWidget *login_window = login_pack->data[0];
    GtkWidget *entry_user = login_pack->data[1];
    GtkWidget *entry_pass = login_pack->data[2];
    GtkApplication *app = login_pack->app;

    const gchar *username = gtk_entry_get_text(GTK_ENTRY(entry_user));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(entry_pass));

    strcpy(login_pack->login_info->username, username);
    strcpy(login_pack->login_info->email, username);

    char hashed_password[65];
    hash_password_sha256(password, hashed_password);
    printf(" on_login_clicked1 : Password: %s\n", password);
    printf(" on_login_clicked1 : Hashed password: %s\n", hashed_password);

    strcpy(login_pack->login_info->password, hashed_password);
    login_pack->login_info->login_register = LOGIN;

    g_print("Attempting login: %s / %s\n", login_pack->login_info->username, login_pack->login_info->password);

    int login_status = login_attempts(login_pack);

    if (login_status == 0)
    {
        gtk_widget_destroy(login_window);
        show_chat_window(login_pack);

        printf("Connexion réussie. Tentative de récupération de l'ID utilisateur...\n");

        PGconn *conn = PQconnectdb("dbname=whispr_db user=my_user password=my_password host=localhost port=5432");

        if (PQstatus(conn) != CONNECTION_OK)
        {
            fprintf(stderr, "Erreur de connexion à la base PostgreSQL : %s\n", PQerrorMessage(conn));
            PQfinish(conn);
            return;
        }

        const char *paramValues[1] = {username};
        printf("Nom d'utilisateur fourni pour la requête : %s\n", username);

        PGresult *res = PQexecParams(conn,
                                     "SELECT id_user FROM users WHERE username = $1 OR email = $1",
                                     1,    // nParams
                                     NULL, // paramTypes
                                     paramValues,
                                     NULL, // paramLengths
                                     NULL, // paramFormats
                                     0     // resultFormat (0 = texte)
        );

        if (res == NULL || PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            fprintf(stderr, "La requête a échoué : %s\n", PQerrorMessage(conn));
            PQclear(res);
            PQfinish(conn);
            return;
        }

        int n = PQntuples(res);
        printf("Nombre de lignes retournées : %d\n", n);

        if (n > 0)
        {
            const char *id_str = PQgetvalue(res, 0, 0);
            printf("ID utilisateur récupéré : %s\n", id_str);

            // Correction de strtol : ajouter un pointeur 'endptr' et spécifier la base (par exemple, 10 pour décimal)
            char *endptr;
            session_user_id = strtol(id_str, &endptr, 10);
            if (*endptr != '\0')
            {
                fprintf(stderr, "Erreur de conversion de l'ID utilisateur\n");
                PQclear(res);
                PQfinish(conn);
                return;
            }
            printf("session_user_id mis à jour : %d\n", session_user_id);
        }
        else
        {
            printf("Aucun utilisateur trouvé avec ce username/email.\n");
        }

        if (res != NULL)
        {
            PQclear(res);
        }
        PQfinish(conn);
    }
}

// === CALLBACK: When the "Register" button is clicked ===
static void on_register_clicked(GtkButton *button, gpointer user_data)
{
    Login_package_for_front *login_pack = (Login_package_for_front *)user_data;

    // GtkWidget **data = user_data;
    GtkWidget *login_window = login_pack->data[0];
    // GtkApplication *app = login_pack->app;

    // GtkWidget **data = user_data;
    // GtkWidget *login_window = data[0];
    // GtkApplication *app = GTK_APPLICATION(data[3]);

    g_print("Register link clicked!\n");

    // Close the login window before opening the registration window
    gtk_widget_destroy(login_window);

    // Opens the registration window
    show_register_window(login_pack);
}

// === MAIN FUNCTION: Creates the login window ===
void show_login_window(Login_package_for_front *login_pack)
{
    // Login_package_for_front *login_pack = login_pack;
    login_pack->client->sock_pointer = client_start();

    load_css(login_pack->app); // Load and apply the CSS

    GtkWidget *window = gtk_application_window_new(login_pack->app);
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
    gtk_widget_set_name(separator, "separator_line"); // Give a custom ID to the separator line
    gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);

    // Create a box for labels and input fields with vertical alignment
    GtkWidget *vbox_center = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); // This box will contain the labels and input fields
    gtk_box_set_homogeneous(GTK_BOX(vbox_center), FALSE);               // Elements can have different sizes

    // Add a spacer above
    GtkWidget *spacer_top = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox_center), spacer_top, TRUE, TRUE, 0); // This pushes elements to the center vertically

    // Create the label for email
    GtkWidget *label_user = gtk_label_new("Email/Username");
    gtk_widget_set_name(label_user, "entry_label");
    gtk_widget_set_halign(label_user, GTK_ALIGN_CENTER); // Center aligned
    gtk_box_pack_start(GTK_BOX(vbox_center), label_user, FALSE, FALSE, 0);

    // Create the input field for email
    GtkWidget *entry_user = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_user), "Email or Username");
    gtk_widget_set_name(entry_user, "Placeholder");
    gtk_widget_set_size_request(entry_user, 500, -1); // Limit width to 500px
    gtk_widget_set_halign(entry_user, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox_center), entry_user, FALSE, FALSE, 0);

    // Create another spacer (if needed) between elements
    GtkWidget *spacer_middle = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox_center), spacer_middle, TRUE, TRUE, 0);

    // Create the label for password
    GtkWidget *label_pass = gtk_label_new("Password");
    gtk_widget_set_name(label_pass, "entry_label");
    gtk_widget_set_halign(label_pass, GTK_ALIGN_CENTER); // Center aligned
    gtk_box_pack_start(GTK_BOX(vbox_center), label_pass, FALSE, FALSE, 0);

    // Create the input field for password
    GtkWidget *entry_pass = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_pass), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(entry_pass), FALSE);
    gtk_widget_set_size_request(entry_pass, 500, -1); // Limit width to 500px
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
    gtk_widget_set_name(btn_login, "left_button");     // Assign CSS ID for button styling
    gtk_widget_set_name(btn_register, "right_button"); // Assign CSS ID for button styling

    // Add buttons to the hbox
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_login, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_register, TRUE, TRUE, 5);

    // Allocate data to pass to the signal function
    login_pack->data = g_new(GtkWidget *, 10);
    login_pack->data[0] = window;
    login_pack->data[1] = entry_user;
    login_pack->data[2] = entry_pass;
    // login_pack->data = data;

    // Connect signals for login and register buttons
    g_signal_connect(btn_login, "clicked", G_CALLBACK(on_login_clicked), login_pack);
    g_signal_connect(btn_register, "clicked", G_CALLBACK(on_register_clicked), login_pack); // Call on_register_clicked

    gtk_widget_show_all(window);
}
