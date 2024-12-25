//
// Created by flynn on 24-12-25.
//

#include "sensor_db.h"

void *reader_thread(void* arg)
{
    printf("One thread start.\n");

    if(file == NULL)
    {
        printf("Error opening file\n");
    }

    sensor_data_t *sensor_data = (sensor_data_t *)malloc(sizeof(sensor_data_t));


    do
    {
        pthread_mutex_lock(&mutex);
        sbuffer_remove(sbuffer, sensor_data);
        pthread_mutex_unlock(&mutex);

        if (read_complete==1)
        {
            printf("One thread closes.\n");
            pthread_exit(NULL);
        }

        if ((sensor_data->id==0)&&(write_complete==0)){}
        else if ((sensor_data->id==0)&&(write_complete==1)){
            read_complete = 1;
            fclose(file);
            printf("One thread closes.\n");
            sbuffer_free(&sbuffer);
            pthread_exit(NULL);
        }
        else if (sensor_data->id!=0)
        {
            fprintf(file, "%d, %f, %ld\n",sensor_data->id,sensor_data->value, sensor_data->ts);
            // printf("Reading sensor data %d %f %ld \n",sensor_data->id,sensor_data->value,sensor_data->ts);
            usleep(25000);
        }
    }while (1);
}
