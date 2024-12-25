//
// Created by flynn on 24-12-25.
//

#include "sensor_db.h"

#include <stdio.h>
#include <stdlib.h>
extern int complete_transfer;

typedef struct
{
    sbuffer_t *sbuffer;
}stormgrData;

void *stormgr(void* arg)
{
    FILE *file = fopen("data.csv", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    stormgrData* stormgrdata = (stormgrData*)arg;
    sbuffer_t *sbuffer = stormgrdata->sbuffer;
    while(1){
        if (complete_transfer==1){break;}
        if(sbuffer_head(sbuffer)!=NULL){
            sensor_data_t *data = (sensor_data_t *)malloc(sizeof(sensor_data_t));
            int sbuffer_state = sbuffer_remove(sbuffer, data,STORAGE);
            if (sbuffer_state==SBUFFER_SUCCESS)
            {
                printf("StorageManager Receive data is: %d - %ld - %f\n",data->id,data->ts,data->value);\
                fprintf(file, "%d  %ld  %f\n",data->id,data->ts,data->value);
            }
        }
    }
    fclose(file);
}
