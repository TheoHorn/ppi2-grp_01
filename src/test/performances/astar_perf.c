#include "../../astar.h"
#include "../../utils/parser_csv.h"
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{

    // Creation file
    FILE *f;
    f = fopen("./src/test/performances/perf_astar", "w");
    if(f == NULL)
   {
      printf("Error!\n");
      exit(1);             
   }

    csv_reader_t reader = create_reader_default(DATASET_PATH_STATIONS);
    station_t stations[DATASET_STATIONS_LINES];
    car_t cars[DATASET_CARS_LINES];
    assert(parse_to_station(&reader, stations) == 0);
    reader = create_reader_default(DATASET_PATH_CARS);
    assert(parse_to_car(&reader, cars) == 0);
    srand(time(NULL));
    for(int i = 0; i < 200; i++){
        car_t *car = &cars[rand() % DATASET_CARS_LINES];
        int depart = rand() % DATASET_STATIONS_LINES;
        int arrivee = rand() % DATASET_STATIONS_LINES;
        printf("%d : %d -> %d\n",i, depart, arrivee);
        while(depart == arrivee){
            depart = rand() % DATASET_STATIONS_LINES;
            arrivee = rand() % DATASET_STATIONS_LINES;
        }
        
        clock_t t=clock();

        station_t** path = path_generation(stations, &stations[depart], &stations[arrivee], DATASET_STATIONS_LINES, car);

        // Timer off  
        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
        if(path != NULL){
            fprintf(f, "%d;%d;%d;%f\n", depart, arrivee, path_size(path, stations[arrivee]), time_taken);
        }
        
       // print_path(path, path_size(path, stations[arrivee]));
        free(path);
    }

    fclose(f);
    free_parsed_car(cars);
    free_parsed_station(stations);
    return 0;
}
