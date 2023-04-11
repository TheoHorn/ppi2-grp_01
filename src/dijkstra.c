#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "station.h"
#include "parser_csv.h"
#include "dijkstra.h"

#define dmax 250
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

// returns the path from the depart to the arrivee (the path is in reverse order)
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
    printf("Path: [");
    for(int i = 0; i < length-1; i++){
        printf("%d, ", path[i]);
    }
    printf("%d]\n", path[length]);
}

// keep only the stations that are at a distance of at most 'autonomy' from the previous station
int decreaseNumberStations(station_t* stations ,int*path, int*newPath, int length, int autonomy){
    int c=0;
    int currentStation = path[length];
    int currentDistance = 0;
    newPath[c] = currentStation;
    for(int i = length-1; i >= 0; i--){
        currentDistance += distance(&stations[currentStation], &stations[path[i]]);
        if(currentDistance > autonomy){
            c++;
            newPath[c] = path[i+1];
            currentStation = path[i+1];
            currentDistance = 0;
        }
    }
    c++;
    newPath[c] = path[0];
    return c;
}

int main() {
    int depart = 1;
    int arrivee = 2200;

    csv_reader_t reader = create_reader_default(DATASET_PATH_STATIONS);
    station_t stations[DATASET_STATIONS_LINES];
    assert(parse_to_station(&reader, stations) == 0);

    int* predecessors = calloc(DATASET_STATIONS_LINES, sizeof(int));
    int* path = malloc(sizeof(int) * DATASET_STATIONS_LINES);
    int* newPath = malloc(sizeof(int) * DATASET_STATIONS_LINES);
   
    clock_t t=0;
    int distMin = dijkstra(stations[depart], stations[arrivee], stations, DATASET_STATIONS_LINES, predecessors);
    t = clock() - t;

    printf("\nMinimum distance between station %d and %d: %d km\n",depart, arrivee, distMin);

    int length = predecessorsToPath(predecessors,path,arrivee);

    // 'path' is the shortest path (in reverse order)
    displayPath(path, length);
    
    int newLength = decreaseNumberStations(stations, path, newPath, length, dmax);

    // 'newPath' is the path to display in order (with less stations)
    displayPath(newPath, newLength);

    printf("\nTime: %fs\n", ((double)t)/CLOCKS_PER_SEC);

    free(newPath);
    free(path);
    free(predecessors);
    free_parsed_station(stations);

    return 0;
}
