#include "../utils/parser_csv.h"
#include "../simulation.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    clock_t t=0;

    int nb_simulations = 20; // nb cars
    int nb_minutes = 1000; // each 10 minutes until nb_minutes

    printf("Charging rate of stations for %d cars (in 10 minute steps)",nb_simulations);

    for(int i=1; i<nb_minutes/10;i++){
        csv_reader_t reader = create_reader_default(DATASET_PATH_STATIONS);
        station_t stations[DATASET_STATIONS_LINES];
        car_t cars[DATASET_CARS_LINES];
        assert(parse_to_station(&reader, stations) == 0);
        reader = create_reader_default(DATASET_PATH_CARS);
        assert(parse_to_car(&reader, cars) == 0);
        
        printf("\nAt %d minutes :", i*10);
        
        int state = current_position(stations, DATASET_STATIONS_LINES, cars, nb_simulations, i*10);
        if(!print_charge_stations(stations, DATASET_STATIONS_LINES)) printf(" - ");
        if(state == 1){
            printf(" All cars arrived at their destination");
            free_parsed_car(cars);
            free_parsed_station(stations);
            break;
        }

        free_parsed_car(cars);
        free_parsed_station(stations);
    }

    t = clock() - t;
    printf("\nTemps d'execution : %f secondes\n", ((float)t)/CLOCKS_PER_SEC);

    return 0;
}
