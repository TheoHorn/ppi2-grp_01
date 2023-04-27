#include "../astar.h"
#include "../utils/parser_csv.h"
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int depart = 20;
    int arrivee = 83;

    csv_reader_t reader = create_reader_default(DATASET_PATH_STATIONS);
    station_t stations[DATASET_STATIONS_LINES];
    car_t cars[DATASET_CARS_LINES];
    assert(parse_to_station(&reader, stations) == 0);
    reader = create_reader_default(DATASET_PATH_CARS);
    assert(parse_to_car(&reader, cars) == 0);

    car_t *car = &cars[0];

    // Timer on
    clock_t t=0;
    data_algo_t *param = malloc(sizeof(data_algo_t));
    param->borne_depart = &stations[depart];
    param->borne_arrivee = &stations[arrivee];
    param->vehicule = car;
    param->min_bat = 0.2;
    param->max_bat = 0.8;
    param->current_bat = 0.5;
    param->tps_recharge = 0.5;
    param->payant = false;

    station_t** path = path_generation(stations, &stations[depart], &stations[arrivee], DATASET_STATIONS_LINES, car, param);
    print_path(path, path_size(path, stations[arrivee]));
    free(path);

    // Timer off  
    t = clock() - t;
    printf("Temps d'execution : %f secondes\n", ((float)t)/CLOCKS_PER_SEC);

    free(param);
    free_parsed_car(cars);
    free_parsed_station(stations);
    return 0;
}
