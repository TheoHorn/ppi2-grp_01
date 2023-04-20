#ifndef SIMULATION_H
#define SIMULATION_H

#include "car.h"
#include "astar.h"

// current position of the car at the time nbMinutes
station_t* current_position(car_t *car, station_t** path, int nbMinutes);

//generate random departure and arrival stations
void generate_departure_arrival_stations(station_t *departure_station, station_t *arrival_station, station_t stations[], int nbstations);

// array of coordinates of cars at the time nbMinutes
char* simulation_at_time(int nbCars, int minute, station_t stations[], int nbstations);

#endif
