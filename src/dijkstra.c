#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <limits.h>

#include "station.h"
#include "parser_csv.h"
#include "dijkstra.h"

#define dmax 345
#define dmin 0

// returns the index of the station with the minimum distance in the array of distances
int min_distance_index(double *distances, bool *visited, int n) {
    double min = DBL_MAX;
    int min_index;
    for (int i = 0; i < n; i++) {
        if (!visited[i] && distances[i] <= min) {
            min = distances[i];
            min_index = i;
        }
    }
    return min_index;
}

double dijkstra(station_t *stations, int nbStations, int start, int end) {
    double distances[nbStations]; // distances from start to each station
    bool visited[nbStations]; // visited stations

    for (int i = 0; i < nbStations; i++) {
        distances[i] = DBL_MAX; // +infinity
        visited[i] = false;
    }
    distances[start] = 0;

    // for each station
    for (int i = 0; i < nbStations - 1; i++) {
        // station with the minimum distance
        int current = min_distance_index(distances, visited, nbStations);
        visited[current] = true;

        // update the distances to all adjacent stations
        for (int j = 0; j < nbStations; j++) {
            if (!visited[j] && distance(&stations[current], &stations[j]) != INT_MAX && distances[current] + distance(&stations[current], &stations[j]) < distances[j]) {
                distances[j] = distances[current] + distance(&stations[current], &stations[j]);
            }
        }
    }

    return distances[end];
}

int main() {
    // tests
    station_t stations[] = {
        {0, "Station A", 0.0, 0.0, 10, 5, true},
        {1, "Station B", 1.0, 1.0, 20, 10, false},
        {2, "Station C", 2.0, 2.0, 30, 15, true},
        {3, "Station D", 3.0, 3.0, 40, 20, false},
        {4, "Station E", 4.0, 4.0, 50, 25, true}
    };

    int n = sizeof(stations) / sizeof(stations[0]); // 5 stations

    int start = 0;
    int end = 4;
    double shortest_distance = dijkstra(stations, n, start, end);

    printf("Dijkstra : min distance between station %d and station %d: %.2f km\n", start, end, shortest_distance);

    return 0;
}
