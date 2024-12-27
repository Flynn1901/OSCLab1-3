#include "datamgr.h"

#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "lib/dplist.h"


#define SIZE 400
#define READ_END 0
#define WRITE_END 1

extern int complete_transfer;
extern pthread_mutex_t mutex;
extern pthread_mutex_t mutex_log;
extern int MAX_CONN;
int total_sensor_num;

typedef struct
{
        sensor_id_t sensor_id;
        room_id_t room_id;
        double running_avg[5];
		int targetelement;
        time_t last_modified;
}my_element_t;

typedef struct
{
	FILE *map;
	sbuffer_t *sbuffer;
	pthread_mutex_t* mutex;
}datamgrData;

static dplist_t *list = NULL;
extern pid_t pid;
extern int fd[2];

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
		int find_or_not = 0;

		while(counter<total_sensor_num){
			my_element_t *current_element = dpl_get_element_at_index(list,counter);
			if(current_element->sensor_id == sensor_id){
					find_or_not = 1;
					current_element->running_avg[current_element->targetelement++] = temperature;
					current_element->last_modified = timestamp;
					if (current_element->targetelement==5) current_element->targetelement=0;
					double running_avg = 0;
					int counter2 = 0;
					for (int i=0;i<5;i++)
					{
						if (current_element->running_avg[i]!=0)
						{
							running_avg += current_element->running_avg[i];
							counter2++;
						}

					}
					running_avg = running_avg/counter2;
					if (running_avg>SET_MAX_TEMP)
					{
						char message2[SIZE];
						snprintf(message2, SIZE, "Sensor node %d reports it's too hot (avg temp = %f)",sensor_id,running_avg);
						pthread_mutex_lock(&mutex_log);
						write(fd[WRITE_END], message2, strlen(message2)+1);
						pthread_mutex_unlock(&mutex_log);
					}
					if (running_avg<SET_MIN_TEMP)
					{
						char message2[SIZE];
						snprintf(message2, SIZE, "Sensor node %d reports it's too cold (avg temp = %f",sensor_id,running_avg);
						pthread_mutex_lock(&mutex_log);
						write(fd[WRITE_END], message2, strlen(message2)+1);
						pthread_mutex_unlock(&mutex_log);
					}
					break;
			}
			counter++;
		}
	if (find_or_not==0)
	{
		char message2[SIZE];
		snprintf(message2, SIZE, " Received sensor data with invalid sensor node ID %d",sensor_id);
		pthread_mutex_lock(&mutex_log);
		write(fd[WRITE_END], message2, strlen(message2)+1);
		pthread_mutex_unlock(&mutex_log);
	}
}


void *datamgr_parse_sensor_files(void* arg){
		datamgrData* tdata = (datamgrData*)arg;
		FILE* fp_sensor_map= tdata->map;
		sbuffer_t *sbuffer = tdata->sbuffer;


		sensor_id_t sensor_id[MAX_CONN];
		room_id_t room_id[MAX_CONN];
		int room_id_turn = 1;
		uint16_t value = 0;
		uint8_t counter = 0;
//Count how many sensors are there in the map file.
		while(fscanf(fp_sensor_map,"%hu",&value)==1){
				if(room_id_turn==1){
						room_id_turn = 0;
				}else{
						room_id_turn = 1;
						counter++;
				}
		}
	total_sensor_num = counter;
	printf("total sensor num = %d\n",total_sensor_num);;

	counter = 0;
	fp_sensor_map = fopen("room_sensor.map", "r");
	while(fscanf(fp_sensor_map,"%hu",&value)==1){
		if(room_id_turn==1){
			printf("Room id is:%d    ",value);
			room_id[counter] = value;
			room_id_turn = 0;
		}else{
			printf("Sensor id is:%d\n",value);
			sensor_id[counter] = value;
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
		while(counter<total_sensor_num){
				content->sensor_id = sensor_id[counter];
				content->room_id = room_id[counter];
				content->last_modified = 0;
				content->targetelement = 0;
				dpl_insert_at_index(list,content,counter,true);
				counter++;
		}
		counter = 0;
		while(counter<total_sensor_num){
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

//try to read the data from sbuffer
		printf("Size of sensor id: %zu\n",sizeof(uint16_t));
		printf("Size of temperature is %zu\n",sizeof(double));
		printf("Size of timestamp is %zu\n",sizeof(time_t));

        while(1){
        		if (complete_transfer==1) break;
        		if(sbuffer_head(sbuffer)!=NULL){
        			sensor_data_t *data = (sensor_data_t *)malloc(sizeof(sensor_data_t));
        			pthread_mutex_lock(&mutex);
        			int sbuffer_state = sbuffer_remove(sbuffer, data,DATA);
        			pthread_mutex_unlock(&mutex);
        			if (sbuffer_state==SBUFFER_SUCCESS)
        			{
        				printf("DataManager Receive data is: %d - %ld - %f\n",data->id,data->ts,data->value);
        				add_new_data(list, data->id, data->value, data->ts);
        			}
                }
        }
// print the final data
		counter = 0;
		while(counter<total_sensor_num){
				my_element_t *current = dpl_get_element_at_index(list, counter);
				printf("Room id is: %d    ",current->room_id);
				printf("Sensor id is: %d   ",current->sensor_id);
				printf("Running avg is: %f   ",(current->running_avg[0]+current->running_avg[1]+current->running_avg[2]+current->running_avg[3]+current->running_avg[4])/5.0);
				printf("Time stamp is: %ld   \n",current->last_modified);
				counter++;
		}
	printf("Done");
	datamgr_free(list);
	return NULL;
}

void datamgr_free(dplist_t *list){
	dpl_free(&list, true);
}

uint16_t datamgr_get_room_id(sensor_id_t sensor_id){
		int counter = 0;
		while(counter<MAX_CONN){
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
		while(counter<MAX_CONN){
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
		while(counter<MAX_CONN){
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

