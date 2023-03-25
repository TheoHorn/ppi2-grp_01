#ifndef PARSER_CSV_H
#define PARSER_CSV_H

#include "car.h"

// struct to easily read a csv file
typedef struct csv_reader_t {
    char *filename; // filename of the csv
    char *separator; // separator of the csv
    int max_line_size; // max length of a line
} csv_reader_t ;

// create a csv reader with the default parameters
csv_reader_t create_reader_default(char* filename);

// create a csv with custom parameters
csv_reader_t create_reader(char* filename, char *separator, int max_line_size);

// parse the csv in the array of car return 0 if done successfully
int parse_to_struct_car(csv_reader_t* reader, car array[]);


#endif