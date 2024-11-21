#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#include <sys/types.h>

#include "sensor_db.h"
#include "logger.h"
#include "config.h"

int main()
{
    FILE *f = open_db("sensor_db.csv", true);

    sensor_id_t id = 1;
    sensor_value_t v = 0.001;
    sensor_ts_t ts = time(NULL);
    insert_sensor(f, id, v, ts);

    // sleep(1);
    id = 2;
    v = 0.002;
    ts = time(NULL);
    insert_sensor(f, id, v, ts);

    // sleep(1);
    id = 3;
    v = 0.003;
    ts = time(NULL);
    insert_sensor(f, id, v, ts);

    // sleep(1);
    ts = time(NULL);
    insert_sensor(f, 4, v, ts);

    // sleep(1);
    close_db(f);
    return 0;
}
