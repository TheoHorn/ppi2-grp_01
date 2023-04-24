#include <gtk/gtk.h>
#include <cairo.h>
#include <ctype.h>

#include "../utils/parser_csv.h"


#define DRAWING_WIDTH 850
#define DRAWING_HEIGHT 550

// --------------------------------------------------- STRUCTURE  -------------------------------------------------- //

/**
 * @brief Structure contenant les widgets pour un label clcikable
 * @param event_box La boîte d'événement
 * @param label Le label
 */
typedef struct{
    GtkWidget *box;
    GtkWidget *button;
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
    char* vehicule;
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
    widget_label->box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    widget_label->button = gtk_button_new_with_label("Use");
    widget_label->label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(widget_label->box),widget_label->label , FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(widget_label->box),widget_label->button , FALSE, FALSE, 0);
    return widget_label;
}

/**
 * @brief Structure contenant les widgets et data de l'interface graphique
*/
typedef struct WD{
    //Widgets principaux
    GtkWidget *window;
    GtkWidget *grid_principal;
    GtkWidget *grid_research;

    //Widgets de la recherhe de stations
    GtkWidget *ss_title;
    GtkWidget *ss_search_entry;
    GtkWidget *ss_search_button;
    GtkWidget *ss_search_grid;
    GtkWidget *ss_search_box;
    WidgetLabel *ss_label1;
    WidgetLabel *ss_label2;
    WidgetLabel *ss_label3;
    WidgetLabel *ss_label4;
    WidgetLabel *ss_label5;
    GtkWidget *box_ss_labels;

    //Widgets de la recherche de voiture
    GtkWidget *sc_title;
    GtkWidget *sc_search_entry;
    GtkWidget *sc_search_button;
    GtkWidget *sc_search_grid;
    GtkWidget *sc_search_box;
    WidgetLabel *sc_label1;
    WidgetLabel *sc_label2;
    WidgetLabel *sc_label3;
    WidgetLabel *sc_label4;
    WidgetLabel *sc_label5;
    GtkWidget *box_sc_labels;

    //Widgets de la carte de france
    GtkWidget *fm_box;
    GtkWidget *fm_drawing_area;

    //Widgets des choix des options de l'utilisateur
    GtkWidget *ou_title;
    GtkWidget *ou_box;
    GtkWidget *ou_grid;
    GtkWidget *ou_label1;
    GtkWidget *ou_spin1;
    GtkWidget *ou_label2;
    GtkWidget *ou_spin2;
    GtkWidget *ou_label3;
    GtkWidget *ou_spin3;
    GtkWidget *ou_label4;
    GtkWidget *ou_spin4;
    GtkWidget *ou_label5;
    GtkWidget *ou_radio_button_box;
    GtkWidget *ou_radio_button_yes, *ou_radio_button_no;
    GtkWidget *ou_button;

    //Widgets de l'affichage des options de l'utilisateur
    GtkWidget *od_box;
    GtkWidget *od_grid;
    GtkWidget *od_titre;
    GtkWidget *od_lbl_vehicule;
    GtkWidget *od_lbl_vehicule_voulu;
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
} WD;

struct Data dataUser;

// ------------------------------------------------- FIN STRUCTURE  ------------------------------------------------ //
//                                                                                                                   //
// ----------------------------------------------- FONCTIONS GENERALES --------------------------------------------- //
/**
 * @brief Initialise les données de l'interface graphique
 * @param wd La structure contenant les widgets et data de l'interface graphique
*/
void initialize(WD *wd){
    //data
    dataUser.borne_depart ="--"; 
    dataUser.borne_arrivee = "--"; 
    dataUser.vehicule = "--";
    dataUser.tps_recharge = "20";
    dataUser.payant = "NON";
    dataUser.min_bat = "5";
    dataUser.max_bat = "95";
    dataUser.current_bat = "50";
    //fin data

    //research stations box
    wd->ss_title = gtk_label_new("Recherche de stations");
    wd->ss_search_entry = gtk_entry_new();
    wd->ss_search_button = gtk_button_new_with_label("Rechercher");
    wd->ss_search_grid = gtk_grid_new();
    wd->ss_search_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(wd->ss_search_box), wd->ss_search_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(wd->ss_search_box), wd->ss_search_button, FALSE, FALSE, 0);
    wd->ss_label1 = init_label();
    wd->ss_label2 = init_label();
    wd->ss_label3 = init_label();
    wd->ss_label4 = init_label();
    wd->ss_label5 = init_label();
    wd->box_ss_labels = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    gtk_container_set_border_width(GTK_CONTAINER(wd->box_ss_labels), 10);
    gtk_box_pack_start(GTK_BOX(wd->box_ss_labels), wd->ss_label1->box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(wd->box_ss_labels), wd->ss_label2->box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(wd->box_ss_labels), wd->ss_label3->box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(wd->box_ss_labels), wd->ss_label4->box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(wd->box_ss_labels), wd->ss_label5->box, FALSE, FALSE, 0);

    gtk_grid_attach(GTK_GRID(wd->ss_search_grid), wd->box_ss_labels, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->ss_search_grid), wd->ss_search_box, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->ss_search_grid), wd->ss_title, 0, 0, 1, 1);
    //fin research stations box

    //research car box
    wd->sc_title = gtk_label_new("Recherche de voiture");
    wd->sc_search_entry = gtk_entry_new();
    wd->sc_search_button = gtk_button_new_with_label("Rechercher");
    wd->sc_search_grid = gtk_grid_new();
    wd->sc_search_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(wd->sc_search_box), wd->sc_search_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(wd->sc_search_box), wd->sc_search_button, FALSE, FALSE, 0);
    wd->sc_label1 = init_label();
    wd->sc_label2 = init_label();
    wd->sc_label3 = init_label();
    wd->sc_label4 = init_label();
    wd->sc_label5 = init_label();
    wd->box_sc_labels = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    gtk_container_set_border_width(GTK_CONTAINER(wd->box_sc_labels), 10);
    gtk_box_pack_start(GTK_BOX(wd->box_sc_labels), wd->sc_label1->box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(wd->box_sc_labels), wd->sc_label2->box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(wd->box_sc_labels), wd->sc_label3->box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(wd->box_sc_labels), wd->sc_label4->box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(wd->box_sc_labels), wd->sc_label5->box, FALSE, FALSE, 0);

    gtk_grid_attach(GTK_GRID(wd->sc_search_grid), wd->box_sc_labels, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->sc_search_grid), wd->sc_search_box, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->sc_search_grid), wd->sc_title, 0, 0, 1, 1);
    //fin research car box

    //france map
    wd->fm_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    wd->fm_drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(wd->fm_drawing_area, DRAWING_WIDTH, DRAWING_HEIGHT);
    gtk_box_pack_start(GTK_BOX(wd->fm_box), wd->fm_drawing_area, FALSE, FALSE, 0);
    //fin france map

    //option user selection
    wd->ou_title = gtk_label_new("Options Utilisateur ");
    wd->ou_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    wd->ou_grid = gtk_grid_new();
    // Créer un GtkAdjustment pour gérer les valeurs du SpinButton
    wd->ou_spin1 = gtk_spin_button_new(gtk_adjustment_new(1.0, 1.0, 100.0, 1.0, 1.0, 0.0), 1.0, 0);
    wd->ou_spin2 = gtk_spin_button_new(gtk_adjustment_new(1.0, 1.0, 100.0, 1.0, 1.0, 0.0), 1.0, 0);
    wd->ou_spin3 = gtk_spin_button_new(gtk_adjustment_new(1.0, 1.0, 100.0, 1.0, 1.0, 0.0), 1.0, 0);
    wd->ou_spin4 = gtk_spin_button_new(gtk_adjustment_new(1.0, 1.0, 120.0, 1.0, 1.0, 0.0), 1.0, 0);
    wd->ou_label1 = gtk_label_new("Batterie minimal souhaité :");
    wd->ou_label2 = gtk_label_new(", maximale :");
    wd->ou_label3 = gtk_label_new("Batterie actuelle :");
    wd->ou_label4 = gtk_label_new("Temps de recharge maximum souhaité (min):");
    wd->ou_label5 = gtk_label_new("Prendre en compte les stations payantes ? :");

    wd->ou_radio_button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(wd->ou_radio_button_box), 10);
    wd->ou_radio_button_yes = gtk_radio_button_new_with_label(NULL, "Oui");
    wd->ou_radio_button_no = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(wd->ou_radio_button_yes), "Non");
    gtk_box_pack_start(GTK_BOX(wd->ou_radio_button_box), wd->ou_radio_button_yes, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(wd->ou_radio_button_box), wd->ou_radio_button_no, TRUE, TRUE, 0);

    wd->ou_button = gtk_button_new_with_label("Validez les modifications");

    gtk_grid_attach(GTK_GRID(wd->ou_grid), wd->ou_title, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(wd->ou_grid), wd->ou_label1, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->ou_grid), wd->ou_spin1, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->ou_grid), wd->ou_label2, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->ou_grid), wd->ou_spin2, 3, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->ou_grid), wd->ou_label3, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->ou_grid), wd->ou_spin3, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->ou_grid), wd->ou_label4, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->ou_grid), wd->ou_spin4, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->ou_grid), wd->ou_label5, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->ou_grid), wd->ou_radio_button_box, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->ou_grid), wd->ou_button, 1, 5, 2, 1);

    gtk_box_pack_start(GTK_BOX(wd->ou_box), wd->ou_grid, TRUE, TRUE, 0);
    //fin option selection

    //option display
    wd->od_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    wd->od_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(wd->od_grid, 10);
    gtk_box_pack_start(GTK_BOX(wd->od_box), wd->od_grid, TRUE, TRUE, 0);

    wd->od_titre = gtk_label_new("Votre trajet");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_titre, 0, 0, 2, 1);

    wd->od_lbl_vehicule = gtk_label_new("Véhicule");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_vehicule, 0, 1, 1, 1);
    wd->od_lbl_vehicule_voulu = gtk_label_new(dataUser.vehicule);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_vehicule_voulu, 0, 2, 1, 1);

    wd->od_lbl_dep = gtk_label_new("Borne de départ ");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_dep, 1, 1, 1, 1);
    wd->od_lbl_dep_voulu = gtk_label_new(dataUser.borne_depart);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_dep_voulu, 1, 2, 1, 1);

    wd->od_lbl_arr = gtk_label_new("Borne d'arrivée ");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_arr, 2, 1, 1, 1);
    wd->od_lbl_arr_voulu = gtk_label_new(dataUser.borne_arrivee);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_arr_voulu, 2, 2, 1, 1);

    wd->od_lbl_min_bat = gtk_label_new("Batterie minimale ");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_min_bat, 3, 1, 1, 1);
    wd->od_lbl_min_voulu = gtk_label_new(dataUser.min_bat);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_min_voulu, 3, 2, 1, 1);

    wd->od_lbl_max_bat = gtk_label_new("Batterie maximale ");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_max_bat, 4, 1, 1, 1);
    wd->od_lbl_max_voulu = gtk_label_new(dataUser.max_bat);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_max_voulu, 4, 2, 1, 1);

    wd->od_lbl_current_bat = gtk_label_new("Batterie actuelle ");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_current_bat, 5, 1, 1, 1);
    wd->od_lbl_current_voulu = gtk_label_new(dataUser.current_bat);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_current_voulu, 5, 2, 1, 1);

    wd->od_lbl_tps_recharge = gtk_label_new("Temps de recharge (min)");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_tps_recharge, 6, 1, 1, 1);
    wd->od_lbl_tps_recharge_voulu = gtk_label_new(dataUser.tps_recharge);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_tps_recharge_voulu, 6, 2, 1, 1);

    wd->od_lbl_station_payante = gtk_label_new("Station payante ");
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_station_payante, 7, 1, 1, 1);
    wd->od_lbl_station_payante_voulu = gtk_label_new(dataUser.payant);
    gtk_grid_attach(GTK_GRID(wd->od_grid), wd->od_lbl_station_payante_voulu, 7, 2, 1, 1);
    //fin option display

    //main
    wd->grid_principal = gtk_grid_new();
    wd->grid_research = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(wd->grid_research), wd->ss_search_grid, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->grid_research), wd->sc_search_grid, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(wd->grid_principal), wd->fm_box, 0, 0, 2, 2);
    gtk_grid_attach(GTK_GRID(wd->grid_principal), wd->grid_research, 2, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(wd->grid_principal), wd->ou_box, 2, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(wd->grid_principal), wd->od_box, 0, 2, 3, 1);
    
    //fin main
}

/**
 * @brief Met à jour les données de l'option display
 * @param wd pointeur sur la structure contenant les widgets
*/
void update_option_display(WD *wd){
    gtk_label_set_text(GTK_LABEL(wd->od_lbl_vehicule_voulu), dataUser.vehicule);
    gtk_label_set_text(GTK_LABEL(wd->od_lbl_dep_voulu), dataUser.borne_depart);
    gtk_label_set_text(GTK_LABEL(wd->od_lbl_arr_voulu), dataUser.borne_arrivee);
    gtk_label_set_text(GTK_LABEL(wd->od_lbl_min_voulu), dataUser.min_bat);
    gtk_label_set_text(GTK_LABEL(wd->od_lbl_max_voulu), dataUser.max_bat);
    gtk_label_set_text(GTK_LABEL(wd->od_lbl_current_voulu), dataUser.current_bat);
    gtk_label_set_text(GTK_LABEL(wd->od_lbl_tps_recharge_voulu), dataUser.tps_recharge);
    gtk_label_set_text(GTK_LABEL(wd->od_lbl_station_payante_voulu), dataUser.payant);
}

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
 * @brief fonction qui ajoute des retour à la ligne pour une chaine de caractères
 * @param str la chaine de caratères
 * @param max_carac le nombre maximum de caractères par ligne
*/
char* to_line(char* str, int max_carac) {
    int len = strlen(str);
    int nb_lignes = len / max_carac + 1;
    char* result = (char*)malloc((nb_lignes * (max_carac + 1) + 1) * sizeof(char));
    int cur_len = 0;
    int last_space = 0;
    int i, j;
    for (i = 0; i < len; i++) {
        if (isspace(str[i])) {
            last_space = i;
        }
        if (isupper(str[i])) {
            cur_len += 2;
        } else {
            cur_len++;
        }
        if (cur_len > max_carac) {
            if (last_space != 0) {
                result[last_space] = '\n';
                cur_len = i - last_space;
                last_space = 0;
            } else {
                result[i] = '\n';
                cur_len = i - last_space;
            }
        }
        result[i] = str[i];
    }
    result[i] = '\0';
    return result;
}

// --------------------------------------------- FIN FONCTIONS GENERALES -------------------------------------------- //
//                                                                                                                    //
// ------------------------------------------------ FONCTION DE DESSIN ---------------------------------------------- //
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

// --------------------------------------------- FIN FONCTION DE DESSIN --------------------------------------------- //
//                                                                                                                    //
// ---------------------------------------- FONCTIONS DE GESTION DES SIGNAUX ---------------------------------------- //

/**
 * @brief fonction qui mets à jour les labels après une recherche
 * @param nb_valide le nombre d'objets valides
 * @param wd la structure de données
 * @param to_print le tableau de chaines de caractères à afficher
 * @param is_station booléen qui indique si on affiche des stations ou des voitures
*/
update_labels_after_research(int nb_valide, WD* wd, char** to_print, bool is_station){
    GtkLabel *label1, *label2, *label3, *label4, *label5;
    GtkButton *button1, *button2, *button3, *button4, *button5;
    
    if(is_station){
        label1 = GTK_LABEL(wd->ss_label1->label);
        label2 = GTK_LABEL(wd->ss_label2->label);
        label3 = GTK_LABEL(wd->ss_label3->label);
        label4 = GTK_LABEL(wd->ss_label4->label);
        label5 = GTK_LABEL(wd->ss_label5->label);

        button1 = GTK_BUTTON(wd->ss_label1->button);
        button2 = GTK_BUTTON(wd->ss_label2->button);
        button3 = GTK_BUTTON(wd->ss_label3->button);
        button4 = GTK_BUTTON(wd->ss_label4->button);
        button5 = GTK_BUTTON(wd->ss_label5->button);
    }else{
        label1 = GTK_LABEL(wd->sc_label1->label);
        label2 = GTK_LABEL(wd->sc_label2->label);
        label3 = GTK_LABEL(wd->sc_label3->label);
        label4 = GTK_LABEL(wd->sc_label4->label);
        label5 = GTK_LABEL(wd->sc_label5->label);

        button1 = GTK_BUTTON(wd->sc_label1->button);
        button2 = GTK_BUTTON(wd->sc_label2->button);
        button3 = GTK_BUTTON(wd->sc_label3->button);
        button4 = GTK_BUTTON(wd->sc_label4->button);
        button5 = GTK_BUTTON(wd->sc_label5->button);
    }
    int nb_carac = 50;
    switch (nb_valide)
    {
    case 0:
        gtk_label_set_text(label1, "Aucun résultat");gtk_widget_hide(button1);
        gtk_label_set_text(label2, "");gtk_widget_hide(button2);
        gtk_label_set_text(label3, "");gtk_widget_hide(button3);
        gtk_label_set_text(label4, "");gtk_widget_hide(button4);
        gtk_label_set_text(label5, "");gtk_widget_hide(button5);
        break;
    case 1:
        //affichage chaine de caractères
        gtk_label_set_text(label1, to_line(to_print[0],nb_carac));gtk_widget_show(button1);
        gtk_label_set_text(label2, "Pas d'autres résultats");gtk_widget_hide(button2);
        gtk_label_set_text(label3, "");gtk_widget_hide(button3);
        gtk_label_set_text(label4, "");gtk_widget_hide(button4);
        gtk_label_set_text(label5, "");gtk_widget_hide(button5);

        break;
    case 2:
       //affichage chaine de caractères
        gtk_label_set_text(label1, to_line(to_print[0],nb_carac));gtk_widget_show(button1);
        gtk_label_set_text(label2, to_line(to_print[1],nb_carac));gtk_widget_show(button2);
        gtk_label_set_text(label3, "Pas d'autres résultats");gtk_widget_hide(button3);
        gtk_label_set_text(label4, "");gtk_widget_hide(button4);
        gtk_label_set_text(label5, "");gtk_widget_hide(button5);
        break;
    case 3:
        //affichage chaine de caractères
        gtk_label_set_text(label1, to_line(to_print[0],nb_carac));gtk_widget_show(button1);
        gtk_label_set_text(label2, to_line(to_print[1],nb_carac));gtk_widget_show(button2);
        gtk_label_set_text(label3, to_line(to_print[2],nb_carac));gtk_widget_show(button3);
        gtk_label_set_text(label4, "Pas d'autres résultats");gtk_widget_hide(button4);
        gtk_label_set_text(label5, "");gtk_widget_hide(button5);
        break;
    case 4:
        //affichage chaine de caractères
        gtk_label_set_text(label1, to_line(to_print[0],nb_carac));gtk_widget_show(button1);
        gtk_label_set_text(label2, to_line(to_print[1],nb_carac));gtk_widget_show(button2);
        gtk_label_set_text(label3, to_line(to_print[2],nb_carac));gtk_widget_show(button3);
        gtk_label_set_text(label4, to_line(to_print[3],nb_carac));gtk_widget_show(button4);
        gtk_label_set_text(label5, "Pas d'autres résultats");gtk_widget_hide(button5);
        break;
    case 5:
        //affichage chaine de caractères
        gtk_label_set_text(label1, to_line(to_print[0],nb_carac));gtk_widget_show(button1);
        gtk_label_set_text(label2, to_line(to_print[1],nb_carac));gtk_widget_show(button2);
        gtk_label_set_text(label3, to_line(to_print[2],nb_carac));gtk_widget_show(button3);
        gtk_label_set_text(label4, to_line(to_print[3],nb_carac));gtk_widget_show(button4);
        gtk_label_set_text(label5, to_line(to_print[4],nb_carac));gtk_widget_show(button5);
        break;
    default:
        //affichage chaine de caractères
        gtk_label_set_text(label1, to_line(to_print[0],nb_carac));gtk_widget_show(button1);
        gtk_label_set_text(label2, to_line(to_print[1],nb_carac));gtk_widget_show(button2);
        gtk_label_set_text(label3, to_line(to_print[2],nb_carac));gtk_widget_show(button3);
        gtk_label_set_text(label4, to_line(to_print[3],nb_carac));gtk_widget_show(button4);
        char* txt5 = malloc(sizeof(char) * 100);
        sprintf(txt5, "et %d stations supplémentaires ...", nb_valide - 4);
        gtk_label_set_text(label5, txt5);gtk_widget_hide(button5);
        break;
    }

}
/**
 * @brief Fonction appelée lorsqu'on clique sur le bouton "Rechercher" de la fenêtre de recherche de station
 * @param button Le bouton "Rechercher"
 * @param user_data Les données utilisateur (ici, le champ de saisie de texte)
*/
void on_search_station_button_clicked(GtkButton *button, gpointer data)
{
    if (button == NULL) return;

    WD *wd = (WD *) data;
    // Récupérer les différents widgets
    GtkEntry *search_entry = GTK_ENTRY(wd->ss_search_entry);

    // Récupérer le texte saisi dans le champ de saisie
    const char *search_text = gtk_entry_get_text(search_entry);

    // Lancer la recherche avec le texte saisi
    csv_reader_t reader = create_reader_default(DATASET_PATH_STATIONS);
    station_t stations[DATASET_STATIONS_LINES];
    char* station_afficher[5];
    parse_to_station(&reader, stations);
    int j = 0;
    for (int i = 0; i < DATASET_STATIONS_LINES; i++)
    {
        if (startsWith(stations[i].name, search_text)){
            if (j<5){
                station_afficher[j] = stations[i].name;
            }
            j++;
        }
    }
    update_labels_after_research(j,wd, station_afficher,true);
    free_parsed_station(stations);
}

/**
 * @brief Fonction appelée lorsqu'on clique sur le bouton "Rechercher" de la fenêtre de recherche de voitures
 * @param button Le bouton "Rechercher"
 * @param user_data Les données utilisateur (ici, le champ de saisie de texte)
*/
void on_search_car_button_clicked(GtkButton *button, gpointer data)
{
    if (button == NULL) return;

    WD *wd = (WD *) data;
    // Récupérer les différents widgets
    GtkEntry *search_entry = GTK_ENTRY(wd->sc_search_entry);

    // Récupérer le texte saisi dans le champ de saisie
    const char *search_text = gtk_entry_get_text(search_entry);

    // Lancer la recherche avec le texte saisi
    csv_reader_t reader = create_reader_default(DATASET_PATH_CARS);
    car_t cars[DATASET_CARS_LINES];
    char* cars_afficher[5];
    parse_to_car(&reader, cars);
    int j = 0;
    for (int i = 0; i < DATASET_CARS_LINES; i++)
    {
        if (startsWith(cars[i].name, search_text)){
            if (j<5){
                cars_afficher[j] = cars[i].name;
            }
            j++;
        }
    }
    update_labels_after_research(j,wd, cars_afficher,false);
    free_parsed_car(cars);
}

/**
 * @brief Fonction qui permet de récupérer les données de l'utilisateur sur les bornes de départ et d'arrivée
 * @param widget Le widget qui a appelé la fonction
 * @param data Les données de l'utilisateur
*/
void on_use_station_clicked(GtkWidget *widget, gpointer data){
    WD *wd = (WD *) data;
    GtkWidget *parent = gtk_widget_get_parent(widget);
    GList *children = gtk_container_get_children(GTK_CONTAINER(parent));
    GtkWidget *first_child = GTK_WIDGET(children->data);
    GtkLabel *label = GTK_LABEL (first_child);

    if (strcmp(dataUser.borne_depart, "--") == 0){
            dataUser.borne_depart = gtk_label_get_text(label);
    }else if (strcmp(dataUser.borne_arrivee, "--") == 0){
            dataUser.borne_arrivee = gtk_label_get_text(label);
    }else{
            dataUser.borne_depart = gtk_label_get_text(label);
            dataUser.borne_arrivee = "--";
    }
    update_option_display(wd);
    g_list_free(children);
}

/**
 * @brief Fonction qui permet de récupérer les données de l'utilisateur sur le véhicule
 * @param widget Le widget qui a appelé la fonction
 * @param data Les données de l'utilisateur
*/
void on_use_car_clicked(GtkWidget *widget, gpointer data){
    WD *wd = (WD *) data;
    GtkWidget *parent = gtk_widget_get_parent(widget);
    GList *children = gtk_container_get_children(GTK_CONTAINER(parent));
    GtkWidget *first_child = GTK_WIDGET(children->data);
    GtkLabel *label = GTK_LABEL (first_child);
    dataUser.vehicule = gtk_label_get_text(label);
    update_option_display(wd);
    g_list_free(children);
}

char* double_to_string(double value){
    char *str = malloc(sizeof(char) * 16);
    snprintf(str, 16, "%.f", value);
    return str;
}
/**
 * @brief Fonction qui permet de récupérer les données de l'utilisateur sur l'option
 * @param widget Le widget qui a appelé la fonction
 * @param data Les données de l'utilisateur
*/
void on_ou_button_clicked(GtkWidget *widget, gpointer data){
    WD *wd = (WD *) data;
    GtkWidget *parent = gtk_widget_get_parent(widget);
    GList *children = gtk_container_get_children(GTK_CONTAINER(parent));

    dataUser.min_bat = double_to_string(gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_list_nth_data(children, 9))));
    dataUser.max_bat = double_to_string(gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_list_nth_data(children, 7))));
    dataUser.current_bat = double_to_string(gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_list_nth_data(children, 5))));
    dataUser.tps_recharge = double_to_string(gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_list_nth_data(children, 3))));

    GtkWidget* radio = GTK_WIDGET(g_list_nth_data(children, 1));
    GList *children_box_radio = gtk_container_get_children(GTK_CONTAINER(radio));
    GtkWidget *first_child = GTK_WIDGET(children_box_radio->data);
    GSList *group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(first_child));
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(group->data))) {
        dataUser.payant = "NON";
    }else{
        dataUser.payant = "OUI";
    }
    update_option_display(wd);
    g_list_free(children);
}
// -------------------------------------- FIN FONCTIONS DE GESTION DES SIGNAUX -------------------------------------- //
//                                                                                                                    //
// ------------------------------------- FONCTIONS DE CONNEXION ET D'AFFICHAGE -------------------------------------- //

/**
 * @brief Fonction qui permet de connecter les signaux aux fonctions
 * @param wd Les données de l'utilisateur
*/
void connect_signals(WD *wd) {
    g_signal_connect(wd->ss_search_button, "clicked", G_CALLBACK(on_search_station_button_clicked), wd);
    g_signal_connect(wd->sc_search_button, "clicked", G_CALLBACK(on_search_car_button_clicked), wd);
    g_signal_connect(G_OBJECT(wd->fm_drawing_area), "draw", G_CALLBACK(on_draw), wd);
    g_signal_connect(wd->ss_label1->button, "clicked", G_CALLBACK(on_use_station_clicked), wd);
    g_signal_connect(wd->ss_label2->button, "clicked", G_CALLBACK(on_use_station_clicked), wd);
    g_signal_connect(wd->ss_label3->button, "clicked", G_CALLBACK(on_use_station_clicked), wd);
    g_signal_connect(wd->ss_label4->button, "clicked", G_CALLBACK(on_use_station_clicked), wd);
    g_signal_connect(wd->ss_label5->button, "clicked", G_CALLBACK(on_use_station_clicked), wd);
    g_signal_connect(wd->sc_label1->button, "clicked", G_CALLBACK(on_use_car_clicked), wd);
    g_signal_connect(wd->sc_label2->button, "clicked", G_CALLBACK(on_use_car_clicked), wd);
    g_signal_connect(wd->sc_label3->button, "clicked", G_CALLBACK(on_use_car_clicked), wd);
    g_signal_connect(wd->sc_label4->button, "clicked", G_CALLBACK(on_use_car_clicked), wd);
    g_signal_connect(wd->sc_label5->button, "clicked", G_CALLBACK(on_use_car_clicked), wd);
    g_signal_connect(wd->ou_button, "clicked", G_CALLBACK(on_ou_button_clicked), wd);
}

/**
 * @brief Fonction qui permet de cacher certains widgets
 * @param wd Les données de l'utilisateur
*/
void widget_to_hide(WD *wd){
    gtk_widget_hide(wd->ss_label1->button);
    gtk_widget_hide(wd->ss_label2->button);
    gtk_widget_hide(wd->ss_label3->button);
    gtk_widget_hide(wd->ss_label4->button);
    gtk_widget_hide(wd->ss_label5->button);
    gtk_widget_hide(wd->sc_label1->button);
    gtk_widget_hide(wd->sc_label2->button);
    gtk_widget_hide(wd->sc_label3->button);
    gtk_widget_hide(wd->sc_label4->button);
    gtk_widget_hide(wd->sc_label5->button);
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
    widget_to_hide(wd);
    g_signal_connect(wd->window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
}

