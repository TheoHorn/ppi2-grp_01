#ifndef STATION_H
#define STATION_H

#include <stdbool.h>


#define RADIAN 0.01745329251994329576923690768489

typedef struct station_t {
    int id; // id of the station
    char *name; // name of the station
    double latitude, longitude; // coordinate with longitude and latitude converted in radian for calculation
    int capacity; // capacity maximum of the station
    int power; // power of the station in kW
    bool is_free; // is the station free

} station_t ;

// return the distance between two station in km
// we use pythagore methode because of performance issue
// and since the France is "small enough" to use this methode without to much mistake
// we get 922.3 km with pythagore and 922.9 with haversine
int distance(station_t *station1, station_t *station2);

#endif
