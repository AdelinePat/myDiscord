PROCEDURE INSTALLATION INTERFACE GRAPHIQUE  

1. Installer WSL(C’est un sous-système Linux intégré à Windows.)  
util pour installer ubuntu  
Ouvre PowerShell avec win+R en administrateur et tape :  
  
```bash
wsl --install  
```  
  
💡 Ça va installer WSL 2 avec Ubuntu automatiquement.  
Redémarre ensuite le PC.  
  
2. Ouvrir Unbuntu(environnemnet de bureau) :   

  - Après le redemarrage du PC, :  
  - Chercher "UBUNTU" dans le menu Démarrer :  
  - Lance-le une première fois, attendre l'installation  
  - Créér son nom utilisateur Linux et MDP  
  
3. Installation outils dans Ubuntu
  
```bash
sudo apt update
sudo apt install build-essential libgtk-3-dev pkg-config
```  
4. ouvrir Vscode avec ubunto :  
Ajouter extension WSL  

A utiliser dans vscode  
```bash
sudo apt update
sudo apt install libgtk-3-dev  
```  
  
  
5. Créer fichier example.c  

```vscode
#include <gtk/gtk.h>

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Hello GTK");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.example.myapp", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}


```  

6. Installation serveur affichage - GTK :   
      
**Installe un serveur d'affichage comme VcXsrv sur ton Windows**   
**parce que linux n'affiche pas nativemnet des interfaces graphiques sous windows**
   
```a copier
https://vcxsrv.com/  
```  
  
Dans Ubuntu, il faut taper :   

```bash
export DISPLAY=:0  
```  

7. Compiler le programme   
Dans le terminal Ubuntu dans VScode, compile avec :  
  
```bash
gcc example.c -o mon_app `pkg-config --cflags --libs gtk+-3.0`  
```  


  
8. Lance l'application avec :  

```bash  
./mon_app
```  

9. Pour les commandes run et compile :    
utiliser le terminal TMX   

  



  
  
