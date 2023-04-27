#include "../utils/parser_csv.h"
#include "../simulation.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    clock_t t=0;

    int nb_simulations = 20;
    int nb_minutes = 100;

    for(int i=0; i<50;i++){
        csv_reader_t reader = create_reader_default(DATASET_PATH_STATIONS);
        station_t stations[DATASET_STATIONS_LINES];
        car_t cars[DATASET_CARS_LINES];
        assert(parse_to_station(&reader, stations) == 0);
        reader = create_reader_default(DATASET_PATH_CARS);
        assert(parse_to_car(&reader, cars) == 0);

        current_position(stations, DATASET_STATIONS_LINES, cars, DATASET_CARS_LINES, nb_simulations, i*10);
        print_charge_stations(stations, DATASET_STATIONS_LINES);

        free_parsed_car(cars);
        free_parsed_station(stations);
    }

    
    t = clock() - t;
    printf("\nTemps d'execution : %f secondes\n", ((float)t)/CLOCKS_PER_SEC);

    return 0;
}
