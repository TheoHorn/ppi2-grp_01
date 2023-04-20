#include "simulation.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

station_t* current_position(car_t *car, station_t** path, int nbMinutes){
    int i = 0;
    /*
    while (path[i] != NULL){
        if (path[i] ==){
            break;
        }
        i++;
    }*/
    int j = 0;
    while (path[i] != NULL){
        if (j == nbMinutes){
            return path[i];
        }
        j++;
        i++;
    }
    return NULL;
}

void generate_departure_arrival_stations(station_t *departure_station, station_t *arrival_station, station_t stations[], int nbstations){
    int i = rand() % nbstations;
    int j = rand() % nbstations;
    while (i == j){
        j = rand() % nbstations;
    }
    *departure_station = stations[i];
    *arrival_station = stations[j];
}

char* simulation_at_time(int nbCars, int minute, station_t stations[], int nbstations){
    char* result = malloc(sizeof(char) * nbstations * nbstations);
    for (int i = 0; i < nbstations * nbstations; i++){
        result[i] = '0';
    }
    for (int i = 0; i < nbCars; i++){
        car_t car;
        //generate_departure_arrival_stations();
        //station_t** path = astar();
        //station_t* current = current_position();
        /*if (current != NULL){
            result[current->id * nbstations + current->id] = '1';
        }*/
    }
    return result;
}