#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "sbuffer.h"
#include <string.h>


#define ERROR (-1)
#define SBUFFER_SIZE 16
#define MAX_TNUM 3
sbuffer_t *sbuffer;
int read_or_not = 0;
int read_complete = 0;
int write_complete = 0;

void *reader_thread(void* arg)
{
    printf("One thread start.\n");
    usleep(20000);
    FILE *file = fopen("sensor_data_out.csv","a");

    if(file == NULL)
    {
        printf("Error opening file\n");
    }

    sensor_data_t *sensor_data = (sensor_data_t *)malloc(sizeof(sensor_data_t));


    do
    {
        if (write_complete == 1&&read_complete==1) { printf("One thread closes.\n"); return NULL; };
        while (read_or_not==1){}
        while (read_or_not==1){}
        read_or_not = 1;
        sbuffer_remove(sbuffer, sensor_data);
        read_or_not=0;

        fprintf(file, "%d %f %ld\n",sensor_data->id,sensor_data->value, sensor_data->ts);
        printf("Reading sensor data %d %f %ld \n",sensor_data->id,sensor_data->value,sensor_data->ts);
        usleep(25000);

    }while (sensor_data->id != 0);
    read_complete = 1;
    fclose(file);
    printf("One thread closes.\n");
    return NULL;
}

void *writer_thread(void* arg)
{
    FILE *fp_sensor_data = fopen("sensor_data", "rb");
    if (fp_sensor_data == NULL) {   printf("Error opening file\n"); exit(-1); }


    uint16_t sensor_id_new = 0;
    double temperature = 0;
    time_t timestamp = 0;
    size_t bytesRead;
    int counter = 0;
    long total_byte = 0;

    fseek(fp_sensor_data, 0, SEEK_END);
    long fileSize = ftell(fp_sensor_data);
    rewind(fp_sensor_data);

    while(total_byte<fileSize)
    {
        sensor_data_t *new_data = (sensor_data_t *)malloc(sizeof(sensor_data_t));
        switch(counter){
        case 0:
            //read sensor_id 2 byte
                bytesRead = fread(&sensor_id_new, 2, 1, fp_sensor_data);
            // 打印读取到的 16 位数据，十六进制输出
            // printf("Read Sensor id is: %d    ", sensor_id_new);
            counter = 1;
            total_byte+=2;
            break;
        case 1:
            //read temperature进行
                bytesRead = fread(&temperature, 8, 1, fp_sensor_data);
            // 打印读取到的8byte数据
            // printf("Read temperature is: %f    ",temperature);
            counter = 2;
            total_byte+=8;
            break;
        case 2:
            //read time stamp
                bytesRead = fread(&timestamp, 8, 1, fp_sensor_data);
            // 打印读取到的8byte数据
            // printf("Read time stamp is: %ld\n", timestamp);
            counter = 0;
            total_byte+=8;
            break;
        default:
            break;
        }

        if(counter==0)
        {
            // printf("New data is : %d, %f, %ld \n",sensor_id_new,temperature,timestamp);
            new_data->id = sensor_id_new;
            new_data->ts = timestamp;
            new_data->value = temperature;

            while (read_or_not==1){}
            read_or_not=1;
            printf("Insert data: %s \n",!sbuffer_insert(sbuffer,new_data)? "Success":"Fail");
            read_or_not=0;
            usleep(10000);
        }

    }

    sensor_id_new = 0;
    timestamp = 0;
    temperature = 0;
    sensor_data_t *new_data = (sensor_data_t *)malloc(sizeof(sensor_data_t));
    new_data->id = sensor_id_new;
    new_data->ts = timestamp;
    new_data->value = temperature;
    printf("Insert final data: %s \n",!sbuffer_insert(sbuffer,new_data)? "Success":"Fail");
    // printf("End data is: %d",sensor_id_new);
    write_complete = 1;
    fclose(fp_sensor_data);
    return NULL;
}


int main(void)
{
    printf("Very complex question\n");

    //initialize the buffer
    sbuffer= malloc(SBUFFER_SIZE);
    printf("%s",!sbuffer_init(&sbuffer)? "OK" : "ERROR");

    pthread_t tid[MAX_TNUM];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tid[0],&attr,writer_thread,NULL);
    pthread_create(&tid[1],&attr,reader_thread,NULL);
    pthread_create(&tid[2],&attr,reader_thread,NULL);

    printf("Wait for thread close: thread %d\n",0);
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    pthread_join(tid[2],NULL);

    return 0;
}