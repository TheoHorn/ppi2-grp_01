#include "../astar.h"
#include "../utils/parser_csv.h"
#include "../simulation.h"
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

print_position(position_t* position){
    printf("Position actuelle : %f,%f\n nbMinutes : %d\n", position->latitude, position->longitude, position->nbMinutes);
}
int main()
{
    int depart = 2000;
    int arrivee = 8317;

    csv_reader_t reader = create_reader_default(DATASET_PATH_STATIONS);
    station_t stations[DATASET_STATIONS_LINES];
    car_t cars[DATASET_CARS_LINES];
    assert(parse_to_station(&reader, stations) == 0);
    reader = create_reader_default(DATASET_PATH_CARS);
    assert(parse_to_car(&reader, cars) == 0);

    car_t *car = &cars[0];

    // Timer on
    clock_t t=0;

    station_t** path = path_generation(stations, &stations[depart], &stations[arrivee], DATASET_STATIONS_LINES, car);
    print_path(path, path_size(path, stations[arrivee]));

    position_t* pos = malloc(sizeof(position_t));
    pos->path = path;
    pos->car = car;
    pos->at_station = true;
    pos->longitude = path[0]->longitude;
    pos->latitude = path[0]->latitude;
    pos->nbMinutes = 0;

    current_position(pos, path_size(pos->path, stations[arrivee]), 10);
    print_position(pos);

    free(pos->path);
    free(pos);

    // Timer off  
    t = clock() - t;
    printf("Temps d'execution : %f secondes\n", ((float)t)/CLOCKS_PER_SEC);

    free_parsed_car(cars);
    free_parsed_station(stations);
    return 0;
}
