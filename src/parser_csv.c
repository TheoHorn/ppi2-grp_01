#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser_csv.h"

csv_reader_t create_reader_default(const char* filename) {
    csv_reader_t reader = {
        .filename = filename,
        .separator = ";",
        .max_line_size = 512};
    return reader;
}

csv_reader_t create_reader(const char* filename, const char *separator, int max_line_size) {
    csv_reader_t reader = {
        .filename = filename, 
        .separator = separator,
        .max_line_size = max_line_size};
    return reader;
}

int parse_to_car(csv_reader_t* reader, car_t array[]) {
    FILE* file = fopen(reader->filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Impossible de trouver le fichier '%s'\n", reader->filename);
        return EXIT_FAILURE;
    }

    char line[reader->max_line_size];
    char* field, *copy;

    int line_index = 0, column_index = 0;
    int first_line = 1;
    while (fgets(line, reader->max_line_size, file) != NULL) {

        // to not add the headers of the csv
        if (first_line) {
            first_line = 0;
            continue;
        }

        field = strtok(line, reader->separator);
        while (field != NULL) {

            // --- parsing
            switch (column_index) {
                case 0:
                    copy = malloc(strlen(field) + 1);
                    strcpy(copy, field);
                    array[line_index].name = copy;
                    break;
                case 1:
                    array[line_index].range = atoi(field);
                    break;
                case 2:
                    array[line_index].battery = atof(field);
                    break;
                case 3:
                    array[line_index].consumption = atoi(field);
                    break;

                default:
                    fprintf(stderr, "Parsing du fichier '%s' colonne en trop\n", reader->filename);
                    return EXIT_FAILURE;
            }
            // --- end

            field = strtok(NULL, reader->separator);
            column_index++;
        }

        column_index = 0;
        line_index++;
    }

    fclose(file);
    return EXIT_SUCCESS;
}

int free_parsed_car(car_t array[]) {
    for (int i = 0; i < DATASET_CARS_LINES; i ++) {
        free(array[i].name);
    }
    return EXIT_SUCCESS;
}
