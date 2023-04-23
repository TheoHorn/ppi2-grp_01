#include "simulation.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <math.h>

#define VITESSE 80 // km/h
#define RADIAN 0.01745329251994329576923690768489

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
    
            printf("coordonnées = %f,%f\n", lat/RADIAN, lon/RADIAN);
            position->latitude = lat;
            position->longitude = lon;
            break;
        }
    }
    // case where the car is at the end of the path
}
