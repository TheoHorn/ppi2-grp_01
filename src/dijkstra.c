#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "station.h"
#include "parser_csv.h"
#include "dijkstra.h"

#define dmax 200
#define dmin 0
#define ININITY 999999

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
        int* voisines = adjacentStations(start, visited, dmax, stations, nbStations);
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

int *adjacentStations(station_t* station, int* visited, int distanceMax, station_t *stations, int nbStations){
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

int predecessorsToPath(int* predecessors, int* path, int arrivee){
    int j=1;
    path[0]=arrivee;
    while(predecessors[arrivee] != -1){
        path[j] = predecessors[arrivee];
        arrivee = predecessors[arrivee];
        j++;
    }
    return j-1;
}

void displayPath(int* path, int length){
    for(int k = length; k >= 1; k--){
        printf("%d-->", path[k]);
    }
    printf("%d", path[0]);
}

int decreaseNumberStations(station_t* stations ,int*path, int*newPath, int length, int autonomy){
    int currentStation = path[0];
    newPath[0] = path[0];
    for(int i = 1; i < length; i++){
        while(autonomy - distance(&stations[currentStation], &stations[path[i]]) > 0 && i < length){
            autonomy = autonomy - distance(&stations[currentStation], &stations[path[i]]);
            i++;
        }
    }

    for(int i = 0; i < length; i++){
        //printf("%d ", newPath[i]);
    }

    return length;
}

int main() {
    int depart = 1;
    int arrivee = 2200;

    csv_reader_t reader = create_reader_default(DATASET_PATH_STATIONS);
    station_t stations[DATASET_STATIONS_LINES];
    assert(parse_to_station(&reader, stations) == 0);

    int* predecessors = calloc(DATASET_STATIONS_LINES, sizeof(int));
   
    clock_t t=0;
    int distMin = dijkstra(stations[depart], stations[arrivee], stations, DATASET_STATIONS_LINES, predecessors);
    t = clock() - t;

    printf("\nDistance min between station %d and station %d: %d km\n",depart, arrivee, distMin);
    printf("Path: ");

    int* path = malloc(sizeof(int) * DATASET_STATIONS_LINES);
    int length = predecessorsToPath(predecessors,path,arrivee);

    int* newPath = malloc(sizeof(int) * DATASET_STATIONS_LINES);
    int newLength = decreaseNumberStations(stations, path, newPath, length, 200);
    free(newPath);

    displayPath(path, length);

    printf("\nDistance directe = %d km", distance(&stations[depart], &stations[arrivee]));
    printf("\nTime: %fs\n", ((double)t)/CLOCKS_PER_SEC);

    free(path);
    free(predecessors);
    free_parsed_station(stations);

    return 0;
}
