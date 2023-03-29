#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "../station.c"

station_t* fill_array() {
    station_t *stations = malloc(sizeof(station_t) * 6);

    stations[0] = (station_t) {
        .id = 1,
        .name = "nancy",
        .latitude = 48.684322 * RADIAN,
        .longitude = 6.177800 * RADIAN,
        .capacity = 1,
        .power = 1,
        .is_free = true
    };

    stations[1] = (station_t) {
        .id = 2,
        .name = "nancy",
        .latitude = 49.124561 * RADIAN,
        .longitude = 6.177870 * RADIAN,
        .capacity = 2,
        .power = 2,
        .is_free = true
    };

    stations[2] = (station_t) {
        .id = 3,
        .name = "quimper",
        .latitude = 47.859397 * RADIAN,
        .longitude = -4.292186 * RADIAN,
        .capacity = 3,
        .power = 3,
        .is_free = true
    };

    stations[3] = (station_t) {
            .id = 4,
            .name = "proche lyon",
            .latitude = 45.808972 * RADIAN,
            .longitude = 4.545520 * RADIAN,
            .capacity = 3,
            .power = 3,
            .is_free = true
    };

    stations[4] = (station_t) {
            .id = 5,
            .name = "dans csv",
            .latitude = 45.975531 * RADIAN,
            .longitude = 4.716682 * RADIAN,
            .capacity = 3,
            .power = 3,
            .is_free = true
    };

    stations[5] = (station_t) {
            .id = 6,
            .name = "dans csv",
            .latitude = 50.6884145 * RADIAN,
            .longitude = 3.1756309 * RADIAN,
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
    assert(stations[3].id == 4);

    assert(distance(&stations[0], &stations[1]) == 49); // 48.95
    assert(distance(&stations[1], &stations[0]) == 49);

    assert(distance(&stations[0], &stations[2]) == 780); // 779.676
    assert(distance(&stations[2], &stations[0]) == 780);

    assert(distance(&stations[0], &stations[3]) == 343); // 342.62
    assert(distance(&stations[3], &stations[0]) == 343);

    assert(distance(&stations[1], &stations[2]) == 784); // 783.609
    assert(distance(&stations[2], &stations[1]) == 784);

    assert(distance(&stations[1], &stations[3]) == 389); // 388.53
    assert(distance(&stations[3], &stations[1]) == 389);

    assert(distance(&stations[3], &stations[2]) == 709); // 709.4
    assert(distance(&stations[3], &stations[2]) == 709);

    assert(distance(&stations[4], &stations[5]) == 536); // 536.255
    assert(distance(&stations[5], &stations[4]) == 536);


}

int main() {
    test_calculate_distance();
    return 0;
}
