#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "lib/dplist.h"
#include "datamgr.h"
#include <time.h>

typedef struct
{
		sensor_id_t sensor_id;
		room_id_t room_id;
		double running_avg;
		time_t last_modified;
} my_element_t;

int main(){
    printf("Hello World\n");

    FILE * map = fopen("room_sensor.map", "r");
    FILE * data = fopen("sensor_data", "rb");

    if(map == NULL) return -1;
    if(data == NULL) return -1;

    datamgr_parse_sensor_files(map, data);

    datamgr_free();

    fclose(map);
    fclose(data);
}
