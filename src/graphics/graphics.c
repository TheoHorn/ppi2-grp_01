#include <gtk/gtk.h>
#include <cairo.h>
#include <ctype.h>

#include "../utils/parser_csv.h"


#define DRAWING_WIDTH 850
#define DRAWING_HEIGHT 550


/**
 * @brief Structure contenant les widgets pour un label clcikable
 * @param event_box La boîte d'événement
 * @param label Le label
 */
typedef struct{
    GtkEventBox *event_box;
    GtkWidget *label;
} WidgetLabel;

/**
 * @brief structure contenant les données d'un trajet
 * @param borne_depart La borne de départ
 * @param borne_arrivee La borne d'arrivée
 * @param min_bat La batterie minimale
 * @param max_bat La batterie maximale
 * @param current_bat La batterie actuelle
 * @param tps_recharge Le temps de recharge max voulu
 * @param payant Si l'on souhaite avoir des recharges payantes ou non
*/
typedef struct Data{
    char* borne_depart;
    char* borne_arrivee;
    char* min_bat;
    char* max_bat;
    char* current_bat;
    char* tps_recharge;
    char* payant;
}Data;

/**
 * @brief Initialise un WidgetLabel
*/
WidgetLabel* init_label(){
    WidgetLabel *widget_label = malloc(sizeof(WidgetLabel));
    widget_label->event_box = gtk_event_box_new();
    widget_label->label = gtk_label_new("");
    gtk_container_add(GTK_CONTAINER(widget_label->event_box), widget_label->label);
    return widget_label;
}

/**
 * @brief Structure contenant les widgets et data de l'interface graphique
*/
typedef struct WD{
    GtkWidget *window;
    GtkWidget *grid_principal;
    GtkWidget *search_entry;
    GtkWidget *search_button;
    GtkWidget *search_grid;
    GtkWidget *search_box;
    WidgetLabel *sb_label1;
    WidgetLabel *sb_label2;
    WidgetLabel *sb_label3;
    WidgetLabel *sb_label4;
    WidgetLabel *sb_label5;
    GtkWidget *box_sb_labels;
    GtkWidget *fm_box;
    GtkWidget *fm_drawing_area;
    GtkWidget *od_box;
    GtkWidget *od_grid;
    GtkWidget *od_titre;
    GtkWidget *od_lbl_dep;
    GtkWidget *od_lbl_dep_voulu;
    GtkWidget *od_lbl_arr;
    GtkWidget *od_lbl_arr_voulu;
    GtkWidget *od_lbl_min_bat;
    GtkWidget *od_lbl_min_voulu;
    GtkWidget *od_lbl_max_bat;
    GtkWidget *od_lbl_max_voulu;
    GtkWidget *od_lbl_current_bat;
    GtkWidget *od_lbl_current_voulu;
    GtkWidget *od_lbl_tps_recharge;
    GtkWidget *od_lbl_tps_recharge_voulu;
    GtkWidget *od_lbl_station_payante;
    GtkWidget *od_lbl_station_payante_voulu;
    char* borne_depart;
    char* borne_arrivee;
    int min_bat;
    int max_bat;
    int current_bat;
    int tps_recharge;
    bool payant;
} WD;


struct Data dataUser;


void initialize(WD *wd){
    //data
    dataUser.borne_depart ="--"; //100 caractères max
    dataUser.borne_arrivee = "--"; //100 caractères max
    dataUser.tps_recharge = "20";
    dataUser.payant = "Non";
    dataUser.min_bat = "5";
    dataUser.max_bat = "95";
    dataUser.current_bat = "50";
    //fin data

    //search box
    wd->search_entry = gtk_entry_new();
    wd->search_button = gtk_button_new_with_label("Rechercher");
    wd->search_grid = gtk_grid_new();
    wd->search_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(wd->search_box), wd->search_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(wd->search_box), wd->search_button, FALSE, FALSE, 0);
    wd->sb_label1 = init_label();
    wd->sb_label2 = init_label();
    wd->sb_label3 = init_label();
    wd->sb_label4 = init_label();
    wd->sb_label5 = init_label();
    wd->box_sb_labels = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    gtk_container_set_border_width(GTK_CONTAINER(wd->box_sb_labels), 10);
    gtk_box_pack_start(GTK_BOX(wd->box_sb_labels), wd->sb_label1->event_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(wd->box_sb_labels), wd->sb_label2->event_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(wd->box_sb_labels), wd->sb_label3->event_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(wd->box_sb_labels), wd->sb_label4->event_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(wd->box_sb_labels), wd->sb_label5->event_box, FALSE, FALSE, 0);

    gtk_grid_attach(GTK_GRID(wd->search_grid), wd->box_sb_labels, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->search_grid), wd->search_box, 0, 0, 1, 1);
    //fin search box

    //france map
    wd->fm_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    wd->fm_drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(wd->fm_drawing_area, DRAWING_WIDTH, DRAWING_HEIGHT);
    gtk_box_pack_start(GTK_BOX(wd->fm_box), wd->fm_drawing_area, FALSE, FALSE, 0);
    //fin france map

    //option display
    wd->od_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    wd->od_grid = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(wd->od_box), wd->od_grid, TRUE, TRUE, 0);

    wd->od_titre = gtk_label_new("Votre trajet");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_titre, 0, 0, 2, 1);

    wd->od_lbl_dep = gtk_label_new("Borne de départ ");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_dep, 0, 1, 1, 1);
    wd->od_lbl_dep_voulu = gtk_label_new(dataUser.borne_depart);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_dep_voulu, 0, 2, 1, 1);

    wd->od_lbl_arr = gtk_label_new("Borne d'arrivée ");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_arr, 1, 1, 1, 1);
    wd->od_lbl_arr_voulu = gtk_label_new(dataUser.borne_arrivee);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_arr_voulu, 1, 2, 1, 1);

    wd->od_lbl_min_bat = gtk_label_new("Batterie minimale ");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_min_bat, 2, 1, 1, 1);
    wd->od_lbl_min_voulu = gtk_label_new(dataUser.min_bat);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_min_voulu, 2, 2, 1, 1);

    wd->od_lbl_max_bat = gtk_label_new("Batterie maximale ");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_max_bat, 3, 1, 1, 1);
    wd->od_lbl_max_voulu = gtk_label_new(dataUser.max_bat);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_max_voulu, 3, 2, 1, 1);

    wd->od_lbl_current_bat = gtk_label_new("Batterie actuelle ");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_current_bat, 4, 1, 1, 1);
    wd->od_lbl_current_voulu = gtk_label_new(dataUser.current_bat);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_current_voulu, 4, 2, 1, 1);

    wd->od_lbl_tps_recharge = gtk_label_new("Temps de recharge ");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_tps_recharge, 5, 1, 1, 1);
    wd->od_lbl_tps_recharge_voulu = gtk_label_new(dataUser.tps_recharge);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_tps_recharge_voulu, 5, 2, 1, 1);

    wd->od_lbl_station_payante = gtk_label_new("Station payante ");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_station_payante, 6, 1, 1, 1);
    wd->od_lbl_station_payante_voulu = gtk_label_new(dataUser.payant);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_station_payante_voulu, 6, 2, 1, 1);
    //fin option display

    //main
    wd->grid_principal = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(wd->grid_principal), wd->fm_box, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->grid_principal), wd->search_grid, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->grid_principal), wd->od_box, 0, 1, 2, 1);
    //fin main
}

// Fonction de dessin

/**
 * Dessine la carte de France
 */
gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    if (widget == NULL || data == NULL) {}
    // Récupérer les données
    csv_reader_t reader = create_reader_default(DATASET_PATH_STATIONS);
    station_t stations[DATASET_STATIONS_LINES];
    parse_to_station(&reader, stations);
    
    // Dessiner la France
    // Dessiner une bordure
    cairo_set_source_rgba(cr, 0,0,0,1);
    cairo_rectangle(cr, 0, 0, DRAWING_WIDTH, DRAWING_HEIGHT);
    cairo_fill (cr);
    // Dessiner un fond coloré
    cairo_set_source_rgb(cr, 0.909, 0.862, 0.792);
    int border_width = 5;
    cairo_rectangle(cr, border_width, border_width, DRAWING_WIDTH - 2*border_width, DRAWING_HEIGHT - 2*border_width);
    cairo_fill (cr);

    int idDepart = -1;
    int idArrivée = -1;
    char * b_dep = dataUser.borne_depart;
    char * b_arr = dataUser.borne_arrivee;
    // Dessiner les stations
    int radius;
    for (int i = 0; i < DATASET_STATIONS_LINES; i++) {
        if (b_dep != NULL && strcmp(stations[i].name, b_dep) == 0){
            idDepart = i;
        }else if(b_arr != NULL && strcmp(stations[i].name, b_arr) == 0){
            idArrivée = i;
        }else{
            //contour noir
            radius = 3;
            cairo_set_source_rgba(cr, 0, 0, 0,1);
            cairo_arc(cr, stations[i].longitude*3000 + 300, 2700 - stations[i].latitude*3000, radius, 0, 2 * G_PI);
            cairo_fill(cr);
            //remplissage orange
            radius = 2;
            cairo_set_source_rgba(cr, 0.7, 0.3, 0,1);
            cairo_arc(cr, stations[i].longitude*3000 + 300, 2700 - stations[i].latitude*3000, radius, 0, 2 * G_PI);
            cairo_fill(cr);
        }
    }

    //affichage départ
    if(idDepart != -1){
        //contour noir
        radius = 6;
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_arc(cr, stations[idDepart].longitude*3000 + 300, 2700 - stations[idDepart].latitude*3000, radius, 0, 2 * G_PI);
        cairo_fill(cr);

        //remplissage vert
        radius = 5;
        cairo_set_source_rgba(cr, 0, 1, 0,1);
        cairo_arc(cr, stations[idDepart].longitude*3000 + 300, 2700 - stations[idDepart].latitude*3000, radius, 0, 2 * G_PI);
        cairo_fill(cr);
    }

    //affichage chemin
    /*if(){
            radius = 6;
            cairo_set_source_rgba(cr, 0, 0, 0,1);
            cairo_arc(cr, chemin[i].longitude*3000 + 300, 2700 - chemin[i].latitude*3000, radius, 0, 2 * G_PI);
            cairo_fill(cr);
            //remplissage bleu
            radius = 5;
            cairo_set_source_rgba(cr, 0, 0, 1,1);
            cairo_arc(cr, chemin[i].longitude*3000 + 300, 2700 - chemin[i].latitude*3000, radius, 0, 2 * G_PI);
            cairo_fill(cr);
        }*/

    //affichage arrivée
    if (idArrivée != -1){
        //contour noir
        radius = 6;
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_arc(cr, stations[idArrivée].longitude*3000 + 300, 2700 - stations[idArrivée].latitude*3000, radius, 0, 2 * G_PI);
        cairo_fill(cr);

        //remplissage jaune
        radius = 5;
        cairo_set_source_rgba(cr, 1, 1, 0,1);
        cairo_arc(cr, stations[idArrivée].longitude*3000 + 300, 2700 - stations[idArrivée].latitude*3000, radius, 0, 2 * G_PI);
        cairo_fill(cr);
    }   
    free_parsed_station(stations);
    return TRUE;
}

//end fonction de dessin

//Fonction de gestion des événements

/**
 * @brief Vérifie si la chaîne de caractères theString commence par la chaîne de caractères theBase en ignorant la casse
 * @param theString La chaîne de caractères à vérifier
 * @param theBase La chaîne de caractères à rechercher
 * @return true si la chaîne de caractères theString commence par la chaîne de caractères theBase, false sinon
*/
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
    bool retour = strncmp( str, base, strlen( base ) ) == 0;
    free(str);
    free(base);
    return retour;
}

/**
 * @brief Fonction appelée lorsqu'on clique sur le bouton "Rechercher"
 * @param button Le bouton "Rechercher"
 * @param user_data Les données utilisateur (ici, le champ de saisie de texte)
*/
void on_search_button_clicked(GtkButton *button, gpointer data)
{
    if (button == NULL) return;

    WD *wd = (WD *) data;
    // Récupérer les différents widgets
    GtkEntry *search_entry = GTK_ENTRY(wd->search_entry);
    GtkLabel *label1 = GTK_LABEL(wd->sb_label1->label);
    GtkLabel *label2 = GTK_LABEL(wd->sb_label2->label);
    GtkLabel *label3 = GTK_LABEL(wd->sb_label3->label);
    GtkLabel *label4 = GTK_LABEL(wd->sb_label4->label);
    GtkLabel *label5 = GTK_LABEL(wd->sb_label5->label);

    // Récupérer le texte saisi dans le champ de saisie
    const char *search_text = gtk_entry_get_text(search_entry);

    // Lancer la recherche avec le texte saisi
    csv_reader_t reader = create_reader_default(DATASET_PATH_STATIONS);
    station_t stations[DATASET_STATIONS_LINES];
    station_t station_afficher[5];
    parse_to_station(&reader, stations);
    int j = 0;
    for (int i = 0; i < DATASET_STATIONS_LINES; i++)
    {
        if (startsWith(stations[i].name, search_text)){
            if (j<5){
                station_afficher[j] = stations[i];
            }
            j++;
        }
    }
    // Afficher les résultats
    switch (j)
    {
    case 0:
        gtk_label_set_text(label1, "Aucun résultat");
        gtk_label_set_text(label2, "");
        gtk_label_set_text(label3, "");
        gtk_label_set_text(label4, "");
        gtk_label_set_text(label5, "");
        break;
    case 1:
        //affichage chaine de caractères
        gtk_label_set_text(label1, station_afficher[0].name);
        gtk_label_set_text(label2, "-");
        gtk_label_set_text(label3, "-");
        gtk_label_set_text(label4, "-");
        gtk_label_set_text(label5, "-");
        break;
    case 2:
       //affichage chaine de caractères
        gtk_label_set_text(label1, station_afficher[0].name);
        gtk_label_set_text(label2, station_afficher[1].name);
        gtk_label_set_text(label3, "-");
        gtk_label_set_text(label4, "-");
        gtk_label_set_text(label5, "-");
        break;
    case 3:
        //affichage chaine de caractères
        gtk_label_set_text(label1, station_afficher[0].name);
        gtk_label_set_text(label2, station_afficher[1].name);
        gtk_label_set_text(label3, station_afficher[2].name);
        gtk_label_set_text(label4, "-");
        gtk_label_set_text(label5, "-");
        break;
    case 4:
        //affichage chaine de caractères
        gtk_label_set_text(label1, station_afficher[0].name);
        gtk_label_set_text(label2, station_afficher[1].name);
        gtk_label_set_text(label3, station_afficher[2].name);
        gtk_label_set_text(label4, station_afficher[3].name);
        gtk_label_set_text(label5, "-");
        break;
    case 5:
        //affichage chaine de caractères
        gtk_label_set_text(label1, station_afficher[0].name);
        gtk_label_set_text(label2, station_afficher[1].name);
        gtk_label_set_text(label3, station_afficher[2].name);
        gtk_label_set_text(label4, station_afficher[3].name);
        gtk_label_set_text(label5, station_afficher[4].name);
        break;
    default:
        //affichage chaine de caractères
        gtk_label_set_text(label1, station_afficher[0].name);
        gtk_label_set_text(label2, station_afficher[1].name);
        gtk_label_set_text(label3, station_afficher[2].name);
        gtk_label_set_text(label4, station_afficher[3].name);
        char* txt5 = malloc(sizeof(char) * 100);
        sprintf(txt5, "et %d stations supplémentaires ...", j);
        gtk_label_set_text(label5, txt5);
        break;
    }
    free_parsed_station(stations);
}


void on_label_clicked(GtkWidget *widget, gpointer data) {
    WD *wd =  (WD *) data;   
    GtkWidget *child = gtk_bin_get_child (GTK_BIN(widget));
    GtkLabel *label = GTK_LABEL (child);
    if (strcmp(dataUser.borne_depart, "--") == 0){
            dataUser.borne_depart = gtk_label_get_text(label);
    }else if (strcmp(dataUser.borne_arrivee, "--") == 0){
            dataUser.borne_arrivee = gtk_label_get_text(label);
    }else{
            dataUser.borne_depart = gtk_label_get_text(label);
            dataUser.borne_arrivee = "--";
    }
    GtkLabel *label_dep = GTK_LABEL(wd->od_lbl_dep_voulu);
    GtkLabel *label_arr = GTK_LABEL(wd->od_lbl_arr_voulu);
    gtk_label_set_text(label_dep, "dp");
    gtk_label_set_text(label_arr, "da");
    /*
    GtkWidget *childg = gtk_grid_get_child_at(GTK_GRID(wd->od_grid), 0, 2);
    if (childg != NULL) {
        gtk_container_remove(GTK_CONTAINER(GTK_GRID(wd->od_grid)), childg);
    }
    wd->od_lbl_dep_voulu = gtk_label_new(dataUser.borne_depart);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_dep_voulu, 0, 2, 1, 1);

    childg = gtk_grid_get_child_at(GTK_GRID(wd->od_grid), 1, 2);
    if (childg != NULL) {
        gtk_container_remove(GTK_CONTAINER(GTK_GRID(wd->od_grid)), childg);
    }
    wd->od_lbl_arr_voulu = gtk_label_new(dataUser.borne_arrivee);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_arr_voulu, 1, 2, 1, 1);
    */
}
//end of fonction de gestion des événements

void connect_signals(WD *wd) {
    g_signal_connect(wd->search_button, "clicked", G_CALLBACK(on_search_button_clicked), wd);
    g_signal_connect(G_OBJECT(wd->fm_drawing_area), "draw", G_CALLBACK(on_draw), wd);
    g_signal_connect(wd->sb_label1->event_box, "button_press_event", G_CALLBACK(on_label_clicked), wd);
    g_signal_connect(wd->sb_label2->event_box, "button_press_event", G_CALLBACK(on_label_clicked), wd);
    g_signal_connect(wd->sb_label3->event_box, "button_press_event", G_CALLBACK(on_label_clicked), wd);
    g_signal_connect(wd->sb_label4->event_box, "button_press_event", G_CALLBACK(on_label_clicked), wd);
    g_signal_connect(wd->sb_label5->event_box, "button_press_event", G_CALLBACK(on_label_clicked), wd);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    WD *wd = malloc(sizeof(WD));

    // Créer une fenêtre
    wd->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(wd->window), "PPII 2023");
    gtk_container_set_border_width(GTK_CONTAINER(wd->window), 10);

    //Initialiser les widgets de l'interface graphique
    initialize(wd);
    connect_signals(wd);
   
    gtk_container_add(GTK_CONTAINER(wd->window), wd->grid_principal);
    gtk_widget_show_all(wd->window);
    g_signal_connect(wd->window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
}
