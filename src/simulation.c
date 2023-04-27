#include "simulation.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <math.h>

#define VITESSE 100 // km/h

void current_position(station_t stations[], int nbStations, car_t car[], int nbCars, int nb_simulations, int nb_minutes){

    int *param_cars = (int*)malloc(sizeof(int)*nb_simulations);
    int *param_departures = (int*)malloc(sizeof(int)*nb_simulations);
    int *param_arrivals = (int*)malloc(sizeof(int)*nb_simulations);

    for(int i=0; i<nb_simulations;i++){
        param_cars[i] = i;
        param_departures[i] = i;
        param_arrivals[i] = 152;
    }

    // discretization of the time
    nb_minutes = nb_minutes/10;
    bool arrived = true;

    // we simulate the position of all the cars
    for(int i=0; i<nb_simulations;i++){
        int voiture = param_cars[i];
        int depart = param_departures[i];
        int arrivee = param_arrivals[i];

        station_t** path = path_generation(stations, &stations[depart], &stations[arrivee], nbStations, &car[voiture]);
        int path_lenght = path_size(path, stations[arrivee]);

        //print_path(path, path_lenght);

        int km = 10*VITESSE/60;

        int add_time = 0;
        arrived = true;

        for(int j = 0; j < path_lenght - 1; j++){
            // number of stops between two stations
            int current_distance = distance(path[j], path[j+1]);
            add_time += (int)current_distance/km;

            double energy_used = (car->consumption * current_distance)/1000;
            double time_to_charge = (energy_used / stations[j+1].power) * 60;

            time_to_charge = round(time_to_charge) / 10;

            // case car arrived
            if(nb_minutes>add_time && path[j+1]->id == arrivee){
                    break;
            }

            // case car at a station
            if(nb_minutes>add_time && nb_minutes<add_time+time_to_charge){
                add_car_to_station(&stations[path[j+1]->id], &car[voiture]);
                break;
            }
            // case car on the road
            else if(nb_minutes<=add_time){
                break;
            }
            add_time += time_to_charge;
        }        
        free(path);
    }
    printf("\nAt %d minutes :", nb_minutes*10);
    
    free_parameters(param_cars, param_departures, param_arrivals);
}

void add_car_to_station(station_t* station, car_t* car){
    if(station->num_cars_charging == station->capacity){
        if(station->car_queue == NULL){
            create_queue_car(station, car);
        } else{
            //add_car_to_queue(station, car);
        }        
    } else{
        station->num_cars_charging += 1;
    }
    return;
}

void free_parameters(int* param_cars, int* param_departures, int* param_arrivals){
    free(param_cars);
    free(param_departures);
    free(param_arrivals);
    return;
}

void print_charge_stations(station_t stations[], int nbStations){
    for(int i=0; i<nbStations;i++){
        if(stations[i].num_cars_charging>0){
            printf("\nCharge stations %d : %d", i, stations[i].num_cars_charging);
            if(stations[i].car_queue != NULL){
                display_queue(stations[i].car_queue);
                destroy_queue(stations[i].car_queue);
            }
        }
    }
    return;
}

void create_queue_car(station_t* station, car_t* car){
    station->car_queue = (list_car_t*)malloc(sizeof(list_car_t));
    station->car_queue->car = car;
    station->car_queue->next = NULL;
    return;
}

void add_car_to_queue(station_t* station, car_t* car){
    list_car_t* queue = (list_car_t*)malloc(sizeof(list_car_t));
    queue->car = car;
    queue->next = NULL;
    list_car_t* aux = station->car_queue;

    while(aux->next != NULL){
        aux = aux->next;
    }
    aux->next = queue;
}

void destroy_queue(list_car_t* queue){
    list_car_t* aux = queue;
    while(aux != NULL){
        list_car_t* next = aux->next;
        free(aux);
        aux = next;
    }
}

void display_queue(list_car_t* queue){
    list_car_t* aux = queue;
    printf(" (full) --> Queue : ");
    while(aux != NULL){
        printf("%s, ", aux->car->name);
        aux = aux->next;
    }
    printf("\n");
}
