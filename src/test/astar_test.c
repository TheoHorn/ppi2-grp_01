#include "../astar.h"
#include "../utils/parser_csv.h"
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    //int depart = 20;
    //int arrivee = 83;
    //int depart = 14772;
    //int arrivee = 12611;
    //int depart = 746;
    //int arrivee = 12070;
    int depart = 17;
    int arrivee = 24;

    csv_reader_t reader = create_reader_default(DATASET_PATH_STATIONS);
    station_t stations[DATASET_STATIONS_LINES];
    car_t cars[DATASET_CARS_LINES];
    assert(parse_to_station(&reader, stations) == 0);
    reader = create_reader_default(DATASET_PATH_CARS);
    assert(parse_to_car(&reader, cars) == 0);

    car_t *car = &cars[0];

    // Timer on
    clock_t t=0;
    data_algo_t *params = malloc(sizeof(data_algo_t));
    params->borne_depart = &stations[depart];
    params->borne_arrivee = &stations[arrivee];
    params->vehicule = car;
    params->min_bat = 0.2;
    params->max_bat = 0.8;
    params->current_bat = 0.5;
    params->tps_recharge = 0.5;
    params->payant = false;

    station_t** path = path_generation(stations, DATASET_STATIONS_LINES, params);
    if(path != NULL){
        print_path(path, path_size(path, stations[arrivee]));
    }
    else{
        printf("Path not found\n");
    }
    free(path);

    // Timer off  
    t = clock() - t;
    printf("Temps d'execution : %f secondes\n", ((float)t)/CLOCKS_PER_SEC);

    free(params);
    free_parsed_car(cars);
    free_parsed_station(stations);
    return 0;
}
