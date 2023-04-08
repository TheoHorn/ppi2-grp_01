#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "car.h"
#include "station.h"

// return the minimum distance between two station in km
double dijkstra(station_t *stations, int nbStations, int start, int end);

// returns the index of the station with the minimum distance in the array of distances
int min_distance_index(double *distances, bool *visited, int n);

#endif
