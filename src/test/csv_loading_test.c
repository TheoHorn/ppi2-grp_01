#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    clock_t start_time = clock();
    csv_reader_t reader = create_reader_default("cars.csv");
    car t[299];
    parse_to_struct_car(&reader, t);
    clock_t end_time = clock(); // get end time
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC; // calculate time taken
    printf("Time taken: %f seconds\n", time_taken);

    for (int i = 0; i < 299; i ++) {
        //printf("%s, %i, %f, %i\n", t[i].name, t[i].range, t[i].battery, t[i].consumption);
        free(t[i].name);
    }

}