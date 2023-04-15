#include "../astar.h"
#include "../utils/parser_csv.h"
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int depart = 1;
    int arrivee = 2200;

    csv_reader_t reader = create_reader_default(DATASET_PATH_STATIONS);
    station_t stations[DATASET_STATIONS_LINES];
    assert(parse_to_station(&reader, stations) == 0);

    clock_t t=0;
    station_t** path = path_generation(stations, &stations[depart], &stations[arrivee], DATASET_STATIONS_LINES);
    t = clock() - t;

    print_path(path, path_size(path, stations[arrivee]));

    free(path);


    printf("Temps d'execution : %f secondes\n", ((float)t)/CLOCKS_PER_SEC);

    free_parsed_station(stations);
    return 0;
}
