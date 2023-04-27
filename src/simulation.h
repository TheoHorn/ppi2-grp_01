#ifndef SIMULATION_H
#define SIMULATION_H

#include "astar.h"
#include "time.h"

// current position of the car at the time nbMinutes
void current_position(station_t stations[], int nbStations, car_t car[], int nbCars, int nb_simulations, int nb_minutes);

// free simulation parameters
void free_parameters(int* param_cars, int* param_departures, int* param_arrivals);

// print the number of cars at each station
void print_charge_stations(station_t stations[], int nbStations);

// add a car to a station
void add_car_to_station(station_t* station, car_t* car);

// initialize the car queue of a station
void create_queue_car(station_t* station, car_t* car);

// add a car to the car queue of a station
void add_car_to_queue(station_t* station, car_t* car);

// destroy the car queue of a station
void destroy_queue(list_car_t* queue);

// display the car queue of a station
void display_queue(list_car_t* queue);


/*
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

//generate random departure and arrival stations
void generate_departure_arrival_stations(station_t *departure_station, station_t *arrival_station, station_t stations[], int nbstations);

// array of coordinates of cars at the time nbMinutes
char* simulation_at_time(int nbCars, int minute, station_t stations[], int nbstations);
*/
#endif
