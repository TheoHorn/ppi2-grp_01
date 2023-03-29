#include <math.h>

#include "station.h"

int distance(station_t *station1, station_t *station2) {

    double x = (station1->longitude - station2->longitude) * cos((station1->latitude + station2->latitude) / 2);
    double y = station1->latitude - station2->latitude;

    double z = sqrt( pow(x, 2) + pow(y, 2) );


    return (int) lround(z * 111.12);
}
