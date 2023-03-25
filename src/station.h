#ifndef STATION_H
#define STATION_H

#include "utils/bool.h"

typedef struct station_t {
    int id; // id of the station
    char *name; // name of the station
    double latitude, longitude; // coordinate with longitude and latitude
    int capacity; // capacity maximum of the station
    int power; // power of the station in kW
    double x, y; // coordinate in x y of the station
    bool is_free; // is the station free

} station_t ;

#endif
