#include "datamgr.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ROOM_SIZE 8

typedef struct 
{
        sensor_id_t sensor_id;
        room_id_t room_id;
        double running_avg[5];
        time_t last_modified;
}my_element_t;

static int running_count[8] = {-1};
static dplist_t *list = NULL;

void *element_copy(void *element)
{
	    my_element_t *copy = malloc(sizeof(my_element_t));
	    assert(copy != NULL);
	    copy->sensor_id = ((my_element_t *)element)->sensor_id;
	    copy->room_id = ((my_element_t *)element)->room_id;
		memcpy(copy->running_avg, ((my_element_t *)element)->running_avg, sizeof(copy->running_avg));
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

void element_free(void **element)
{
		free(*element);
		*element = NULL;
}

void add_new_data(dplist_t* list, sensor_id_t sensor_id, double temperature, time_t timestamp){
		int counter = 0;
		int sensor_id_pro = 0;
		switch(sensor_id)
		{
		case 15:
			sensor_id_pro = 0;
			break;
		case 21:
			sensor_id_pro = 1;
			break;
		case 37:
			sensor_id_pro = 2;
			break;
		case 49:
			sensor_id_pro = 3;
			break;
		case 112:
			sensor_id_pro = 4;
			break;
		case 129:
			sensor_id_pro = 5;
			break;
		case 132:
			sensor_id_pro = 6;
			break;
		case 143:
			sensor_id_pro = 7;
			break;
		default:
			break;
		}

		running_count[sensor_id_pro]++;
		if(running_count[sensor_id_pro]==5)
		{
			running_count[sensor_id_pro]=0;
		}
		while(counter<ROOM_SIZE){
			my_element_t *current_element = dpl_get_element_at_index(list,counter);
			if(current_element->sensor_id == sensor_id){
					current_element->running_avg[running_count[sensor_id_pro]]= +temperature;
					current_element->last_modified = timestamp;
					break;
			}
			counter++;
		}
}


void datamgr_parse_sensor_files(FILE *fp_sensor_map,FILE *fp_sensor_data){
		sensor_id_t sensor_id[ROOM_SIZE];
		room_id_t room_id[ROOM_SIZE];
		int room_id_turn = 1; 
		uint16_t value = 0;
		uint8_t counter = 1;
//read the sensor id and room id from the txt file
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

//using sensor id and room id to do the basic configuration of linked list
		dpl_free(&list,false);
		list = dpl_create(element_copy,element_free,element_compare);
		my_element_t *content = (my_element_t *)malloc(sizeof(my_element_t));
//write sensor and room id to linked list.
		counter = 0;
		while(counter<ROOM_SIZE){
				content->sensor_id = sensor_id[counter];
				content->room_id = room_id[counter];
				// content->running_avg = {0,0,0,0,0};
				content->last_modified = 0;
				dpl_insert_at_index(list,content,counter,true);
				counter++;
		}
		counter = 0;
		while(counter<ROOM_SIZE){
				my_element_t *current = dpl_get_element_at_index(list, counter); 
				printf("Room id is: %d    ",current->room_id);
				printf("Sensor id is: %d   ",current->sensor_id);
				for(int i=0;i<5;i++)
				{
					printf("running_avg[%d] = %lf ",i,current->running_avg[i]);
				}
				printf("Time stamp is: %ld   \n",current->last_modified);
				counter++;
		}

//try to read the data from binary file
		printf("Size of sensor id: %zu\n",sizeof(uint16_t));
		printf("Size of temperature is %zu\n",sizeof(double));
		printf("Size of timestamp is %zu\n",sizeof(time_t));

		uint16_t sensor_id_new = 0;
		double temperature = 0;
		time_t timestamp = 0;
		size_t bytesRead; 
		counter = 0;
		long total_byte = 0;
		int running_count = 0;
		

		fseek(fp_sensor_data, 0, SEEK_END);
	    long fileSize = ftell(fp_sensor_data);
    	rewind(fp_sensor_data);

		while(total_byte<fileSize)
		{
				switch(counter){
						case 0:
								//read sensor_id 2 byte
								bytesRead = fread(&sensor_id_new, 2, 1, fp_sensor_data);
    	    					// 打印读取到的 16 位数据，十六进制输出
        						printf("Read Sensor id is: %d    ", sensor_id_new);
								counter = 1;
								total_byte+=2;
								break;
						case 1:
								//read temperature进行 
								bytesRead = fread(&temperature, 8, 1, fp_sensor_data); 
    	    					// 打印读取到的8byte数据
        						printf("Read temperature is: %f    ",temperature);
								counter = 2;
								total_byte+=8;
								break;
						case 2:
								//read time stamp
								bytesRead = fread(&timestamp, 8, 1, fp_sensor_data); 
    	    					// 打印读取到的8byte数据
        						printf("Read time stampis: %ld\n", timestamp);
								counter = 0;
								total_byte+=8;
								break;
						default:
								break;
				}
				if(counter==0)
				{
					add_new_data(list,sensor_id_new,temperature,timestamp);
				}
		}	
// print the final data
		counter = 0;
		while(counter<ROOM_SIZE){
				my_element_t *current = dpl_get_element_at_index(list, counter); 
				printf("Room id is: %d    ",current->room_id);
				printf("Sensor id is: %d   ",current->sensor_id);
				printf("Running avg is: %f   ",(current->running_avg[0]+current->running_avg[1]+current->running_avg[2]+current->running_avg[3]+current->running_avg[4])/5.0);
				printf("Time stamp is: %ld   \n",current->last_modified);
				counter++;
		}
	printf("Done");
}

void datamgr_free(void){

}

uint16_t datamgr_get_room_id(sensor_id_t sensor_id){
		int counter = 0;
		while(counter<ROOM_SIZE){
				my_element_t *current_element = dpl_get_element_at_index(list,counter);
				if(current_element->sensor_id == sensor_id){
						return current_element->room_id;
				}
		counter++;
		}
		return 0;	
}

sensor_value_t datamgr_get_avg(sensor_id_t sensor_id){
		sensor_value_t a1 = 0.0;
		int counter = 0;
		while(counter<ROOM_SIZE){
				my_element_t *current_element = dpl_get_element_at_index(list,counter);
				if(current_element->sensor_id == sensor_id){
						return (current_element->running_avg[0]+current_element->running_avg[1]+current_element->running_avg[2]+current_element->running_avg[3]+current_element->running_avg[4])/5;
				}
		counter++;
		}
		return a1;
}

time_t datamgr_get_last_modified(sensor_id_t sensor_id){
		time_t t1 = 0;
		int counter = 0;
		while(counter<ROOM_SIZE){
			my_element_t *current_element = dpl_get_element_at_index(list,counter);
			if(current_element->sensor_id == sensor_id){
				return current_element->last_modified;
			}
		counter++;
		}
		return t1;
}

int datamgr_get_total_sensors(void){
		int a = 8;
		return a;
}

