#include "../utils/parser_csv.h"
#include "../simulation.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    /*int nbSimulations = 20;
    int minutes = 10;*/

    csv_reader_t reader = create_reader_default(DATASET_PATH_STATIONS);
    station_t stations[DATASET_STATIONS_LINES];
    car_t cars[DATASET_CARS_LINES];
    assert(parse_to_station(&reader, stations) == 0);
    reader = create_reader_default(DATASET_PATH_CARS);
    assert(parse_to_car(&reader, cars) == 0);

    current_position(stations, DATASET_STATIONS_LINES, cars, DATASET_CARS_LINES, 14);

/*
    // Timer on
    clock_t t=0;

    position_t** positions = simulation_at_time(nbSimulations, minutes, cars, DATASET_CARS_LINES, stations, DATASET_STATIONS_LINES);

    FILE* f = fopen("test.txt", "w");
        fprintf(f, "depart; LATITUDE LONGITUDE ; arrivee ; car");
        for(int i = 0; i < nbSimulations; i++){
            fprintf(f, "%f %f - %f %f - %f %f - %s\n",
                positions[i]->path[0]->latitude/RADIAN,
                positions[i]->path[0]->longitude/RADIAN,
                positions[i]->latitude/RADIAN,
                positions[i]->longitude/RADIAN,
                positions[i]->path[positions[i]->path_size-1]->latitude/RADIAN,
                positions[i]->path[positions[i]->path_size-1]->longitude/RADIAN,
                positions[i]->car->name);
        }

    for(int i = 0; i < nbSimulations; i++){
        print_position(positions[i]);
        printf("car : %s\n range : %d\n consumption : %d\n battery : %d\n", positions[i]->car->name, positions[i]->car->range, positions[i]->car->consumption, positions[i]->car->battery);
    }

    // Timer off  
    t = clock() - t;
    printf("\nTemps d'execution : %f secondes\n", ((float)t)/CLOCKS_PER_SEC);

    free_positions(positions, nbSimulations);*/
    free_parsed_car(cars);
    free_parsed_station(stations);
    return 0;
}
