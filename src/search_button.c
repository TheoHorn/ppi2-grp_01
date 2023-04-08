#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include "parser_csv.h"

// Fonction qui vérifie si la chaîne de caractères theString commence par la chaîne de caractères theBase en ignorant la casse
bool startsWith( const char * theString, const char * theBase ) {
    char* str = malloc(sizeof(char) * strlen(theString) + 1);
    char* base = malloc(sizeof(char) * strlen(theBase) + 1);
    strcpy(str, theString);
    strcpy(base, theBase);
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
    for (int i = 0; base[i] != '\0'; i++) {
        base[i] = tolower(base[i]);
    }
    return strncmp( str, base, strlen( base ) ) == 0;
}

// Fonction appelée lorsqu'on clique sur le bouton "Rechercher"
void on_search_button_clicked(GtkButton *button, gpointer user_data)
{
    // Récupérer le champ de saisie de texte (GtkEntry)
    GtkEntry *search_entry = GTK_ENTRY(user_data);

    // Récupérer le texte saisi dans le champ de saisie
    const char *search_text = gtk_entry_get_text(search_entry);

    // Lancer la recherche avec le texte saisi
    csv_reader_t reader = create_reader_default(DATASET_PATH_STATIONS);
    station_t stations[DATASET_STATIONS_LINES];
    parse_to_station(&reader, stations);
    g_print("Recherche de stations commençant par \"%s\"...\n", search_text);
    for (int i = 0; i < DATASET_STATIONS_LINES; i++)
    {
        if (startsWith(stations[i].name, search_text)){
            g_print("%s\n", stations[i].name);
        }
    }
    g_print("Fin de la recherche.\n");
}

void set_label_texts(GtkWidget *box, char **new_labels, int num_labels) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(box));
    for (int i = 0; i < num_labels && i < g_list_length(children); i++) {
        GtkWidget *child = GTK_WIDGET(g_list_nth_data(children, i));
        gtk_label_set_text(GTK_LABEL(child), new_labels[i]);
    }
    g_list_free(children);
}


int main(int argc, char *argv[])
{
    // Initialiser GTK+
    gtk_init(&argc, &argv);

    // Créer une fenêtre, un champ de saisie de texte et un bouton
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *search_entry = gtk_entry_new();
    GtkWidget *search_button = gtk_button_new_with_label("Rechercher");

    // Création d'un GtkGrid pour ajouter les deux boxes
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Créer une boîte horizontale pour les éléments de la barre de recherche
    GtkWidget *search_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(search_box), search_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(search_box), search_button, FALSE, FALSE, 0);


    // Création des labels
    GtkWidget *label1 = gtk_label_new("Label 1");
    GtkWidget *label2 = gtk_label_new("Label 2");
    GtkWidget *label3 = gtk_label_new("Label 3");
    GtkWidget *label4 = gtk_label_new("Label 4");
    GtkWidget *label5 = gtk_label_new("Label 5");

    // Création d'un conteneur vertical pour les labels
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);
    gtk_box_pack_start(GTK_BOX(box), label1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), label2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), label3, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), label4, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), label5, FALSE, FALSE, 0);

    gtk_grid_attach(GTK_GRID(grid), box, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), search_box, 0, 0, 1, 1);
    

    // Connecter la fonction de rappel au signal "clicked" du bouton "Rechercher"
    g_signal_connect(search_button, "clicked", G_CALLBACK(on_search_button_clicked), search_entry);

    // Afficher la fenêtre et lancer la boucle d'événements GTK+
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
