#include "simulation.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <math.h>

#define VITESSE 100 // km/h

void current_position(station_t stations[], int nbStations, car_t car[], int nbCars, int temps_numero_n){
    int nb_simulations = 20;

    // we can manage which car we want to simulate and their departure and arrival stations
    int param_cars[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    int param_departures[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    int param_arrivals[20] = {1000,1001,1002,1003,1004,1005,1006,1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,1017,1018,1019};

    int* charge_stations = malloc(sizeof(int)*nbStations);

    // we simulate the position of all the cars
    for(int i=0; i<nb_simulations;i++){
        int voiture = param_cars[i];
        int depart = param_departures[i];
        int arrivee = param_arrivals[i];

        station_t** path = path_generation(stations, &stations[depart], &stations[arrivee], nbStations, &car[voiture]);
        int path_lenght = path_size(path, stations[arrivee]);

        int km = 10*VITESSE/60;

        int add_time = 0;
        bool arrived = true;

        for(int j = 0; j < path_lenght - 1; j++){
            // number of stops between two stations
            int current_distance = distance(path[j], path[j+1]);
            add_time += (int)current_distance/km;

            // time to recharge
            double temps_recharge = car->battery/stations[j+1].power;
            int stop_time = temps_recharge*6;

            // check if the car is on the road or if it is charging
            if(temps_numero_n>add_time && temps_numero_n<add_time+stop_time){
                charge_stations[path[j+1]->id] += 1;
                arrived = false;
                break;
            }
            else if(temps_numero_n<=add_time){
                arrived = false;
                break;
            }
            add_time += stop_time;
        }        
        free(path);
    }
    printf("At %d minutes :\n", temps_numero_n*10);
    for(int i=0; i<nbStations;i++){
        if(charge_stations[i]>0) printf("Station %d : %d\n", i, charge_stations[i]);
    }
    free(charge_stations);
}
/*
position_t** simulation_at_time(int nbSimulations, int minute, car_t cars[], int nbcars, station_t stations[], int nbstations){
    position_t** positions = malloc(nbSimulations * sizeof(position_t*));
    int departure = 0;
    int arrival = 0;
    srand(time(NULL));

    for(int i = 0; i < nbSimulations; i++){
        
        departure = generate_departure_station(nbstations);
        arrival = generate_arrival_station(nbstations, departure);

        positions[i] = malloc(sizeof(position_t));
        positions[i]->car = &cars[rand() % nbcars];
        positions[i]->path = path_generation(stations, &stations[departure], &stations[arrival], nbstations, positions[i]->car);
        positions[i]->arrived = false;
        positions[i]->at_station = false;
        int length_path = path_size(positions[i]->path, stations[arrival]);
        positions[i]->path_size = length_path;
        //current_position(positions[i], length_path, minute);
    }
    return positions;
}

int generate_departure_station(int nbstations){
    return rand() % nbstations;
}

int generate_arrival_station(int nbstations, int departure){
    int arrival = rand() % nbstations;
    while(arrival == departure){
        arrival = rand() % nbstations;
    }
    return arrival;
}
*/
/*
void current_position(position_t* position, int nbStations, int nbMinutes){
    int nbKm = nbMinutes * VITESSE / 60; // distance traveled in km
    position->nbMinutes = nbMinutes;

    double rapport=0.00;
    int current_distance=0;
    
    int dist = 0;
    for(int i = 0; i < nbStations - 1; i++){
        current_distance = distance(position->path[i], position->path[i+1]);
        dist += current_distance;
        if(dist > nbKm){
            dist -= current_distance;
            rapport = (double)(nbKm - dist) / (double)distance(position->path[i], position->path[i+1]);
            double lat = position->path[i]->latitude + rapport * (position->path[i+1]->latitude - position->path[i]->latitude);
            double lon = position->path[i]->longitude + rapport * (position->path[i+1]->longitude - position->path[i]->longitude);
    
            position->latitude = lat;
            position->longitude = lon;
            return;
        }
    }
    position->latitude = position->path[nbStations-1]->latitude;
    position->longitude = position->path[nbStations-1]->longitude;
    position->arrived = true;
    position->at_station = true;
    return;
}*/
