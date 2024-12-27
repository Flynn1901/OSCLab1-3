//
// Created by flynn on 24-12-25.
//

#include "sensor_db.h"

#include <string.h>
#include <unistd.h>
#define SIZE 400
#define READ_END 0
#define WRITE_END 1

extern int complete_transfer;
extern pthread_mutex_t mutex;
extern pthread_mutex_t mutex_log;
extern pid_t pid;
extern int fd[2];

typedef struct
{
    sbuffer_t *sbuffer;
    pthread_mutex_t *mutex;
}stormgrData;

void *stormgr(void* arg)
{
    FILE *file = fopen("data.csv", "w");
    char message2[SIZE];
    snprintf(message2, SIZE, "A new data.csv file has been created");
    pthread_mutex_lock(&mutex_log);
    write(fd[WRITE_END], message2, strlen(message2)+1);
    pthread_mutex_unlock(&mutex_log);
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
            pthread_mutex_lock(&mutex);
            int sbuffer_state = sbuffer_remove(sbuffer, data,STORAGE);
            pthread_mutex_unlock(&mutex);
            if (sbuffer_state==SBUFFER_SUCCESS)
            {
                printf("StorageManager Receive data is: %d - %ld - %f\n\n\n\n\n",data->id,data->ts,data->value);\
                fprintf(file, "%d  %ld  %f\n",data->id,data->ts,data->value);
                char message4[SIZE];
                snprintf(message4, SIZE, "Data insertion from sensor %d succeeded",data->id);
                pthread_mutex_lock(&mutex_log);
                write(fd[WRITE_END], message4, strlen(message4)+1);
                pthread_mutex_unlock(&mutex_log);
            }
            free(data);
        }
    }
    fclose(file);
    char message3[SIZE];
    snprintf(message3, SIZE, "A new data.csv file has been closed");
    pthread_mutex_lock(&mutex_log);
    write(fd[WRITE_END], message3, strlen(message3)+1);
    pthread_mutex_unlock(&mutex_log);

    return NULL;
}
