#include "../astar.h"
#include "../utils/parser_csv.h"
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int depart = 2123;
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
    free(path);

    // Timer off  
    t = clock() - t;
    printf("Temps d'execution : %f secondes\n", ((float)t)/CLOCKS_PER_SEC);
    

    free_parsed_car(cars);
    free_parsed_station(stations);
    return 0;
}
