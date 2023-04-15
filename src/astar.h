#include "station_node_priority_queue.h"


station_t** path_generation(station_t stations[], station_t *starting_station, station_t *last_station, int nbstations);

station_t** reconstruct_path(station_node *node);

double calculate_heuristic(station_node *node, station_t *goal);

station_node **adjacentStations(station_t stations[], station_node *node, int nbStations, double distMax);
