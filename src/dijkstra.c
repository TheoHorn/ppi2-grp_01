#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <limits.h>
#include <assert.h>

#include "station.h"
#include "parser_csv.h"
#include "dijkstra.h"

#define dmax 200
#define dmin 0
#define ININITY 999999

// return the minimum distance between two station in km
int dijkstra(station_t depart, station_t arrivee, station_t *stations, int nbStations, int *predecessors){
    int* distances = calloc(nbStations, sizeof(int));

    for(int i = 0; i < nbStations; i++){
        distances[i] = ININITY;
        predecessors[i] = -1;
    }

    int* visited = calloc(nbStations, sizeof(int)); // 0 indicates that the station has not been visited
    int current_station = depart.id;
    station_t* start = &depart;

    for(int i = 0; i < nbStations; i++){
        visited[current_station] = 1;
        int* voisines = stationVoisines(start, visited, dmax, stations, nbStations);
        updateNeighbors(start, voisines, distances, stations, predecessors);

        int minValue = min_distance_index(distances, visited, nbStations);

        freeVoisines(voisines);

        if(minValue == -1){
            int resultat = distances[arrivee.id];
            free(visited);
            free(distances);
            return resultat;
        }

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
    csv_reader_t reader = create_reader_default(DATASET_PATH_STATIONS);
    station_t stations[DATASET_STATIONS_LINES];
    assert(parse_to_station(&reader, stations) == 0);
    
    int n = sizeof(stations) / sizeof(stations[0]); // number of stations

    int* predecessors = calloc(n, sizeof(int));

    int depart = 1;
    int arrivee = 2200;
   
    clock_t t=0;
    int distMin = dijkstra(stations[depart], stations[arrivee], stations, n, predecessors);
    t = clock() - t;

    printf("\nDistance min between station %d and station %d: %d km\n",depart, arrivee, distMin);
    printf("Path: ");
    int i = arrivee;
    int d=0;
    while(predecessors[i] != -1){
        d += distance(&stations[i], &stations[predecessors[i]]);
        printf("%d <- ", i);
        i = predecessors[i];
    }
    printf("%d\nDistance directe = %d km", i, d);
    printf("\nTime: %fs\n", ((double)t)/CLOCKS_PER_SEC);

    free(predecessors);
    free_parsed_station(stations);

    return 0;
}
