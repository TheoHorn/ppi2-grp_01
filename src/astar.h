#include "station_node_priority_queue.h"
#include "car.h"

station_t** path_generation(station_t stations[], station_t *starting_station, station_t *last_station, int nbstations, car_t *car);

station_t** reconstruct_path(station_node *node);

double calculate_heuristic(station_node *node, station_t *goal);

station_node **adjacentStations(station_t stations[], station_node *node, int nbStations, station_t *last_station, double distMax, double distMin);

int path_size(station_t **path, station_t last_station);

void print_path(station_t **path, int nbstations);
