#ifndef SIMULATION_H
#define SIMULATION_H

#include "car.h"
#include "station.h"

typedef struct position_t {
    station_t** path; // path of the car
    double latitude, longitude; // coordinate with longitude and latitude converted in radian for calculation
    car_t *car; // car at the position
    int nbMinutes; // time of the position
    bool arrived; // is the car arrived
    bool at_station; // is the car at a station
} position_t;

// current position of the car at the time nbMinutes
void current_position(position_t* position, int nbStations, int nbMinutes);
/*
//generate random departure and arrival stations
void generate_departure_arrival_stations(station_t *departure_station, station_t *arrival_station, station_t stations[], int nbstations);

// array of coordinates of cars at the time nbMinutes
char* simulation_at_time(int nbCars, int minute, station_t stations[], int nbstations);
*/
#endif
