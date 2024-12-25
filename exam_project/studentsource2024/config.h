/**
 * \author {AUTHOR}
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>
#include <time.h>

typedef uint16_t sensor_id_t,room_id_t;
typedef double sensor_value_t;
typedef time_t sensor_ts_t;         // UTC timestamp as returned by time() - notice that the size of time_t is different on 32/64 bit machine

typedef struct {
    sensor_id_t id;
    sensor_value_t value;
    sensor_ts_t ts;
    int canberemoved;
} sensor_data_t;

#define DATA 0
#define STORAGE 1

#endif /* _CONFIG_H_ */
