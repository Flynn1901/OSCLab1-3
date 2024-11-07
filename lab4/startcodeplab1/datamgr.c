#include "datamgr.h"
#include <stdio.h>
#include <stdbool.h>

#define ROOM_SIZE 8

typedef struct
{
        sensor_id_t sensor_id;
        room_id_t room_id;
        double running_avg;
        time_t last_modified;
} my_element_t;

void *element_copy(void *element)
{
	    my_element_t *copy = malloc(sizeof(my_element_t));
	    assert(copy != NULL);
	    copy->sensor_id = ((my_element_t *)element)->sensor_id;
	    copy->room_id = ((my_element_t *)element)->room_id;
	    copy->running_avg = ((my_element_t *)element)->running_avg;
	    copy->last_modified = ((my_element_t *)element)->last_modified;
		return (void *)copy;
}

int element_compare(void *x, void *y)
{ 
        my_element_t *e1 = (my_element_t *)x;
        my_element_t *e2 = (my_element_t *)y;
        if(e1->sensor_id==e2->sensor_id&&e1->room_id==e2->room_id){
                return 1;
        }
        else{
                return -1;
        }
}


void datamgr_parse_sensor_files(FILE *fp_sensor_map,FILE *fp_sensor_data){
		sensor_id_t sensor_id[ROOM_SIZE];
		room_id_t room_id[ROOM_SIZE];
		int room_id_turn = 1; 
		uint16_t value = NULL;
		uint8_t counter = 1;

		while(fscanf(fp_sensor_map,"%hu",&value)==1){
				if(room_id_turn==1){
						printf("Room id is:%d    ",value);
						room_id[counter-1] = value;
						room_id_turn = 0;
				}else{
						printf("Sensor id is:%d\n",value);
						sensor_id[counter-1] = value;
						room_id_turn = 1;
						counter++;
				}
		}
		//Check the matrix
		while((counter--)-1){
				printf("Room id is:%d   ",room_id[8-counter]);
				printf("Sensor id is:%d\n",sensor_id[8-counter]);
		}
}

void datamgr_free(void){

}

uint16_t datamgr_get_room_id(sensor_id_t sensor_id){
		uint16_t nmr = 1;

		return nmr;	
}

sensor_value_t datamgr_get_avg(sensor_id_t sensor_id){
		sensor_value_t a1 = 0.0;
		
		return a1;
}

time_t datamgr_get_last_modified(sensor_id_t sensor_id){
		time_t t1 = NULL;
		return t1;
}

int datamgr_get_total_sensors(void){
		int a = 1;
		return a;
}

