#include "astar.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Generate a fast path from the starting station to the last station
station_t** path_generation(station_t stations[], station_t *starting_station, station_t *last_station, int nbstations, car_t *car){
    // Initialisation of the queues
    station_node_queue *closedList = create_queue(); 
    station_node_queue *openList = create_queue();
    station_node *starting_node = create_station_node(starting_station, 0, 0);

    // TODO : Prendre les options en paramètre
    double min_battery = 0.25;
    double max_battery = 0.4;
    double current_battery = 0.9;
    double max_time = 20;

    // Car battery before the start
    starting_node->battery_after_charge = current_battery;

    add_to_queue(openList, starting_node);
    while(!queue_is_empty(openList)){
        station_node *node = unqueue(&openList);

        // If the last station is in the open list, we have found the path
        if(node->station->id == last_station->id){ 
            free_queue(openList);
            station_t **path = reconstruct_path(node);
            free(node);
            free_queue(closedList);
            return path;
        }
        else{
            // Calculation the distance max and min with the user options and the current battery
            double distMax = car->range * node->battery_after_charge - car->range * min_battery;
            double distMin = car->range * node->battery_after_charge - car->range * max_battery;

            // Calculation of the adjacents stations (between dmax and dmin distance)
            station_node **neighbours = adjacentStations(stations, node, nbstations, last_station, distMax, distMin);
            
            int i = 0;
            while(neighbours[i]->station != NULL){
                station_node *n = neighbours[i];

                // If the station is not in the closed list and the station is not in the open list with a lower cost, we add it to the open list
                // Else, we free the node because we have better solution
                if(!is_in_queue(closedList, n->station) && !is_in_queue_with_lower_cost(openList, n)){
                    n->heuristic = calculate_heuristic(n, last_station);

                    // Calculation of the battery before the charge
                    double current_battery = node->battery_after_charge * car->battery - distance(n->station, node->station) * (car->consumption / 1000);
                    
                    // Calculation of the battery after the charge for max_time minutes in the station
                    n->battery_after_charge = fmin((max_time/60 * n->station->power + current_battery)/car->battery, 1);
                    
                    add_to_queue(openList, n);
                }
                else{
                    free(n);
                }
                i++;
            }

            free(neighbours[i]);
            free(neighbours);

            add_to_queue(closedList, node);
        }
    }
    // If the open list is empty, no one path exists
    free_queue(openList);
    free_queue(closedList);
    return NULL; 
} 

// Calculate the stations in range of the current station
station_node **adjacentStations(station_t stations[], station_node *node, int nbStations, station_t *last_station, double distMax, double distMin){
    station_node **neighbours = malloc(sizeof(station_node) * nbStations);
    int nbNeighbours = 0;

    // If the last station is in range, we skip the others
    if(distance(node->station, last_station) <= distMax){
        nbNeighbours = 1;
        neighbours[0] = create_station_node(last_station, node->cost + distance(node->station, last_station), 0);
        neighbours[0]->parent = (struct station_node_t*) node;
    }
    else{
        for(int i = 0; i < nbStations; i++){
            double dist = distance(node->station, &stations[i]);
            if(dist >= distMin && dist <= distMax){
                neighbours[nbNeighbours] = create_station_node(&stations[i], node->cost + dist, 0);
                neighbours[nbNeighbours]->parent = (struct station_node_t*) node;
                nbNeighbours++;
            }
        }
    }
    // This station is used to know when the array is finished
    neighbours[nbNeighbours] = create_station_node(NULL, 0, 0);
    return neighbours;
}

// Calculate an heuristic for the A* algorithm
double calculate_heuristic(station_node *node, station_t *goal) {
    return node->cost + distance(node->station, goal);
}


// Recreate the path from the last station to the starting station
station_t** reconstruct_path(station_node *node){
    int nbStations = 0;
    station_node *current = node;
    while(current->parent != NULL){
        nbStations++;
        current = current->parent;
    }
    station_t **path = malloc(sizeof(station_t*) * (nbStations + 1));
    current = node;
    for(int i = nbStations; i >= 1; i--){
        path[i] = current->station;
        current = current->parent;
    }
    path[0] = current->station;
    return path;
}

int path_size(station_t **path, station_t last_station){
    int i = 0;
    while(path[i]->id != last_station.id){
        i++;
    }
    return i + 1;
}

void print_path(station_t **path, int nbstations){
    for(int i = 0; i < nbstations - 1; i++){
        printf("%d -(%dkm)-> ", path[i]->id, (int)distance(path[i], path[i+1]));
    }
    printf("%d\n", path[nbstations - 1]->id);
}