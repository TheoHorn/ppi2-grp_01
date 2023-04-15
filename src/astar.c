#include "astar.h"
#include <stdlib.h>


station_t** path_generation(station_t stations[], station_t *starting_station, station_t *last_station, int nbstations){
    station_node_queue *closedList = create_queue(); 
    station_node_queue *openList = create_queue();
    station_node *starting_node = create_station_node(starting_station, 0, 0);
    add_to_queue(openList, starting_node);
    while(!queue_is_empty(openList)){
        station_node *node = unqueue(openList);
        if(node->station->id == last_station->id){
            free_queue(openList);
            free_queue(closedList);
            free(node); // TODO remove when return reconstruct path
            return NULL;
            //return reconstruct_path(node);  // End calcul here
        }
        else{
            station_node **neighbours = adjacentStations(stations, node, nbstations, 200);
            int i = 0;

            while(neighbours[i]->station != NULL){ // foreach n in neighbours
                station_node *n = neighbours[i];

                if(!is_in_queue(closedList, n->station) && !is_in_queue_with_lower_cost(openList, n)){
                    n->heuristic = calculate_heuristic(n, last_station);
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
    free_queue(openList);
    free_queue(closedList);
    return NULL; // Error here, no path found
} 


station_node **adjacentStations(station_t stations[], station_node *node, int nbStations, double distMax){
    station_node **neighbours = malloc(sizeof(station_node) * nbStations);
    int nbNeighbours = 0;
    for(int i = 0; i < nbStations; i++){
        double dist = distance(node->station, &stations[i]);
        if(dist <= distMax){
            neighbours[nbNeighbours] = create_station_node(&stations[i], node->cost + dist, 0);
            nbNeighbours++;
        }
    }
    neighbours[nbNeighbours] = create_station_node(NULL, 0, 0);
    return neighbours;
}


double calculate_heuristic(station_node *node, station_t *goal) {
    return node->cost + distance(node->station, goal);
}


station_t** reconstruct_path(station_node *node){
    station_t **path = malloc(sizeof(station_t) * 20);
    int i = 0;
    while(node != NULL){
        path[i] = node->station;

        i++;
    }
    return path;
}
