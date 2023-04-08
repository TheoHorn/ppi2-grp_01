#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <limits.h>

#include "station.h"
#include "parser_csv.h"
#include "dijkstra.h"

#define dmax 7000
#define dmin 0
#define ININITY 999999

// return the minimum distance between two station in km
int dijkstra(station_t depart, station_t arrivee, station_t *stations, int nbStations, int *predecessors){
    int* distances = calloc(nbStations, sizeof(int));

    for(int i = 0; i < nbStations; i++){
        distances[i] = ININITY; // DBL_MAX is the maximum value of a double
        predecessors[i] = -1;
    }

    int* visited = calloc(nbStations, sizeof(int)); // 0 indicates that the station has not been visited
    int current_station = depart.id;
    station_t* start = &depart;

    for(int i = 0; i < nbStations; i++){
        visited[current_station] = 1;
        int* voisines = stationVoisines(start, visited, dmax, stations, nbStations);
        updateNeighbors(start, voisines, distances, stations, predecessors);

        /*printf("\ncurrent_station : %d", current_station);
        for(int i = 0; i < nbStations; i++){
            printf("visited : %d\n", visited[i]);
        }
        for(int i = 0; i < nbStations; i++){
            printf("distances : %d\n", distances[i]);
        }*/

        int minValue = min_distance_index(distances, visited, nbStations);

        //printf("\nminValue : %d", minValue);

        freeVoisines(voisines);

        if(minValue == -1){
            int resultat = distances[arrivee.id];
            free(visited);
            free(distances);
            return resultat;
        }

        /*if(minValue == arrivee.id){
            int resultat = distances[arrivee.id];
            free(visited);
            free(distances);
            return resultat;
        }*/
        start = &stations[minValue];
        current_station = minValue;
    }
    int resultat = distances[arrivee.id];

    free(visited);
    free(distances);

    return resultat;
}

// station = station de départ ; voisines = tableau des stations voisines
void updateNeighbors(station_t* station, int* voisines, int* distances, station_t* stations, int* predecessors){
    int i = 0;
    while(voisines[i] != -1){
        int d = distance(station,&stations[voisines[i]]);
        if(distances[station->id] != ININITY) d = d + distances[station->id];
        
        if(d < distances[voisines[i]]){
            distances[voisines[i]] = d;
            predecessors[voisines[i]] = station->id;
        }
        i++;
    }
}

int *stationVoisines(station_t* station, int* visited, int distanceMax, station_t *stations, int nbStations){
    int *voisines = malloc(sizeof(int) * nbStations);
    int nbVoisines = 0;
    for(int i = 0; i < nbStations; i++){
        if(i != station->id){
            int d = distance(station, &stations[i]);
            if(d<=distanceMax && visited[i] == 0){
                voisines[nbVoisines] = i;
                nbVoisines++;
            }
        }
    }
    voisines[nbVoisines] = -1;
    return voisines;
}

void freeVoisines(int* voisines){
    free(voisines);
}

int min_distance_index(int *distances, int *visited, int n){
    double min = ININITY;
    int index = -1;
    for(int i = 0; i < n; i++){
        if(distances[i] < min && visited[i] == 0){
            min = distances[i];
            index = i;
        }
    }
    return index;
}

int main() {
    // tests
    station_t stations[] = {
        {0, "OuestCharge - E-Twin - Saint-Hilaire-de-Loulay - Giraudet", -1.9111, 48.0405, 2, 22, false},
        {1, "Balleroy-sur-Drôme; Rue du Sapin", -0.9795, 48.5623, 2, 22, false},
        {2, "CCTLB VAXAINVILLE", 6.0347, 48.9048, 2, 22000, false},
        {3, "Intermarché ANNEYRON", 5.1986, 45.1247, 3, 22, false},
        {4, "Riorges; Parking Place Jean Cocteau", 4.2472, 45.9422, 2, 22, false},
        {5, "SIEG63 - ePremium - Clermont Ferrand - Salins", 2.7689, 46.4069, 2, 25, false}
    };

    int n = sizeof(stations) / sizeof(stations[0]); // 5 stations

    int* predecessors = calloc(n, sizeof(int));

    int distMin = dijkstra(stations[0], stations[2], stations, n, predecessors);

    printf("\nDistance min between station 0 and station 5: %d km\n", distMin);
    printf("Path: ");
    int i = 5;
    while(predecessors[i] != -1){
        printf("%d <- ", i);
        i = predecessors[i];
    }
    printf("%d", i);

    free(predecessors);
    return 0;
}
