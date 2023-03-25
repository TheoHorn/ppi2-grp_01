#include <time.h>
#include <stdio.h>
#include <assert.h>

#include "../parser_csv.c"
#include "utils.c"

int main() {
    clock_t start_time = clock();
    csv_reader_t reader = create_reader_default(DATASET_PATH_CARS);
    car_t cars[DATASET_CARS_LINES];

    assert (parse_to_car(&reader, cars) == 0);
    printf("Parsing of 'cars.csv' into car_t[] done successfully\n");

    clock_t end_time = clock();
    double time_taken_parse_cars = print_time_taken(start_time, end_time);
    printf("Time taken to parse cars: %f seconds\n", time_taken_parse_cars);

    assert(free_parsed_car(cars) == 0);
    printf("Freeing of car_t[] done successfully\n");
}
