#include <gtk/gtk.h>
#include <cairo.h>

#include "../utils/parser_csv.h"


#define DRAWING_WIDTH 850
#define DRAWING_HEIGHT 550

int DEPART = 1;
int ARRIVEE = 2200;

// Fonction de dessin
gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    
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
    // Dessiner les stations
    int radius;
    for (int i = 0; i < DATASET_STATIONS_LINES; i++) {
        if (stations[i].id == DEPART){
            idDepart = i;
        }else if(stations[i].id == ARRIVEE){
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
    /*if(stations[i].id == 2851 || stations[i].id == 6145 || stations[i].id == 9148 || stations[i].id == 14995 || stations[i].id == 10176 || stations[i].id == 9798 || stations[i].id == 7967 || stations[i].id == 14202 || stations[i].id == 10677){
            radius = 6;
            cairo_set_source_rgba(cr, 0, 0, 0,1);
            cairo_arc(cr, stations[i].longitude*3000 + 300, 2700 - stations[i].latitude*3000, radius, 0, 2 * G_PI);
            cairo_fill(cr);
            //remplissage bleu
            radius = 5;
            cairo_set_source_rgba(cr, 0, 0, 1,1);
            cairo_arc(cr, stations[i].longitude*3000 + 300, 2700 - stations[i].latitude*3000, radius, 0, 2 * G_PI);
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


    
    return TRUE;
}

void on_button_clicked_dep(GtkWidget *button, gpointer data) {
    // Récupérer la valeur sélectionnée dans la liste déroulante
    GtkComboBox *combo_box = GTK_COMBO_BOX(data);
    gint active = gtk_combo_box_get_active(combo_box);

    if (active >= 0) {
        // Récupérer la chaîne de caractères correspondante
        GtkTreeModel *model = gtk_combo_box_get_model(combo_box);
        GtkTreeIter iter;
        gtk_tree_model_iter_nth_child(model, &iter, NULL, active);
        gchar *selected_value;
        gtk_tree_model_get(model, &iter, 0, &selected_value, -1);
        g_print("Selected value: %s", selected_value);
        DEPART = atoi(selected_value);
        GtkWidget *drawing_area = GTK_WIDGET(data);
        // Met à jour la zone de dessin
        gtk_widget_queue_draw_area(drawing_area, 0, 0, DRAWING_WIDTH, DRAWING_HEIGHT);
    }  
}

void on_button_clicked_arr(GtkWidget *button, gpointer data) {
    // Récupérer la valeur sélectionnée dans la liste déroulante
    GtkComboBox *combo_bx = GTK_COMBO_BOX(data);
    gint active = gtk_combo_box_get_active(combo_bx);

    if (active >= 0) {
        // Récupérer la chaîne de caractères correspondante
        GtkTreeModel *model = gtk_combo_box_get_model(combo_bx);
        GtkTreeIter iter;
        gtk_tree_model_iter_nth_child(model, &iter, NULL, active);
        gchar *selected_value;
        gtk_tree_model_get(model, &iter, 0, &selected_value, -1);
        g_print("Selected value: %s", selected_value);
        ARRIVEE = atoi(selected_value);
        GtkWidget *drawing_area = GTK_WIDGET(data);
        // Met à jour la zone de dessin
        gtk_widget_queue_draw_area(drawing_area, 0, 0, DRAWING_WIDTH, DRAWING_HEIGHT);
    }  
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Créer une fenêtre
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "PPII 2023");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Créer un conteneur pour les widgets
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Créer l'image de la France
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, DRAWING_WIDTH, DRAWING_HEIGHT);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw), NULL);
    gtk_box_pack_start(GTK_BOX(box), drawing_area, FALSE, FALSE, 0);

    // Créer une liste déroulante
    GtkListStore *list_store = gtk_list_store_new(1, G_TYPE_STRING);
    GtkTreeIter iter;
    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter, 0, "1", -1);
    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter, 0, "2", -1);
    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter, 0, "3", -1);
    
    GtkWidget *combo_box = gtk_combo_box_new_with_model(GTK_TREE_MODEL(list_store));
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_box), renderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_box), renderer, "text", 0, NULL);
    gtk_box_pack_start(GTK_BOX(box), combo_box, FALSE, FALSE, 0);

    GtkWidget *combo_bx = gtk_combo_box_new_with_model(GTK_TREE_MODEL(list_store));
    GtkCellRenderer *rendrr = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_bx), rendrr, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_bx), rendrr, "text", 0, NULL);
    gtk_box_pack_start(GTK_BOX(box), combo_bx, FALSE, FALSE, 0);

    // Créer un bouton de validation
    GtkWidget *button = gtk_button_new_with_label("Valider");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked_dep), combo_box);
    g_signal_connect(button,"clicked", G_CALLBACK(on_button_clicked_arr), combo_bx);
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

    // Afficher la fenêtre
    gtk_widget_show_all(window);

    // Démarrer la boucle principale GTK
    gtk_main();

    return 0;
}

