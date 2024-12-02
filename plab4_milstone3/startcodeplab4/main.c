#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "sbuffer.h"


#define ERROR (-1)
#define SBUFFER_SIZE 16
#define MAX_TNUM 3
sbuffer_t *sbuffer;

void reader_thread(void)
{

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
            printf("Read time stamp is: %ld\n", timestamp);
            counter = 0;
            total_byte+=8;
            break;
        default:
            break;
        }
        if(counter==0)
        {
            printf("New data is : %d, %f, %ld \n",sensor_id_new,temperature,timestamp);
        }
        usleep(10000);
    }
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

    printf("Wait for thread close: thread %d\n",0);
    pthread_join(tid[0],NULL);

    return 0;
}