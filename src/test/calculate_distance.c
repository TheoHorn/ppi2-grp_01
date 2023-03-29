#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "../station.c"

station_t* fill_array() {
    station_t *stations = malloc(sizeof(station_t) * 3);

    stations[0] = (station_t) {
        .id = 1,
        .name = "Station 1",
        .latitude = 48.684322,
        .longitude = 6.177800,
        .capacity = 1,
        .power = 1,
        .is_free = true
    };

    stations[1] = (station_t) {
        .id = 2,
        .name = "Station 2",
        .latitude = 49.124561,
        .longitude = 6.177870,
        .capacity = 2,
        .power = 2,
        .is_free = true
    };

    stations[2] = (station_t) {
        .id = 3,
        .name = "Station 3",
        .latitude = 47.859397,
        .longitude = -4.292186,
        .capacity = 3,
        .power = 3,
        .is_free = true
    };

    return stations;
}

void free_array(station_t *stations) {
    free(stations);
}

void test_calculate_distance() {
    station_t *stations = fill_array();
    assert(stations != NULL);
    assert(stations[0].id == 1);
    assert(stations[1].id == 2);
    assert(stations[2].id == 3);

    assert(distance(&stations[0], &stations[1]) == 49);
    assert(distance(&stations[1], &stations[0]) == 49);
    printf("%i\n", distance(&stations[1], &stations[2]));
    assert(distance(&stations[1], &stations[2]) == 784);
    assert(distance(&stations[2], &stations[1]) == 784);

    /*assert(distance(&stations[2], &stations[0]) == 922);
    assert(distance(&stations[0], &stations[2]) == 922);*/

}

int main() {
    test_calculate_distance();
    return 0;
}
