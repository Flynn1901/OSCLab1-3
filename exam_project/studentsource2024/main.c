#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "sbuffer.h"
#include <string.h>
#include <inttypes.h>
#include "config.h"
#include "lib/tcpsock.h"
#include "connmgr.h"
#include "datamgr.h"
#include "sensor_db.h"

#define ERROR (-1)
#define SBUFFER_SIZE 16

typedef struct
{
    tcpsock_t *client;
    tcpsock_t *server;
} threadData;

typedef struct
{
  int MAX_CONN;
  int PORT;
  sbuffer_t *sbuffer;
} connmgrdata;

typedef struct
{
    FILE *map;
    sbuffer_t *sbuffer;
}datamgrData;

typedef struct
{
    sbuffer_t *sbuffer;
}stormgrData;

int MAX_CONN;
int PORT;
sbuffer_t *sbuffer;
extern int conn_counter;
int complete_transfer=0;

int main(int argc, char *argv[]) {
    //Configure the sensor number;
    if(argc < 3) {
        printf("Please provide the right arguments: first the port, then the max nb of clients");
        return -1;
    }
    MAX_CONN = atoi(argv[2]);
    PORT = atoi(argv[1]);
    FILE * map = fopen("room_sensor.map", "r");

    sbuffer= malloc(SBUFFER_SIZE);
    printf("%s",!sbuffer_init(&sbuffer)? "OK" : "ERROR");

    pthread_t tid[3];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    connmgrdata connmgrdata1 = {MAX_CONN,PORT,sbuffer};
	pthread_create(&tid[0],&attr,run_connmgr,(void*)&connmgrdata1);
    datamgrData datamgrdata = {map,sbuffer};
    pthread_create(&tid[1],&attr,datamgr_parse_sensor_files,(void*)&datamgrdata);
    stormgrData stormgrdata = {sbuffer};
    pthread_create(&tid[1],&attr,stormgr,(void*)&stormgrdata);


    printf("Wait for thread close: Connection Magnager\n");
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);

    printf("Close all threads\n");

    return 0;
}
