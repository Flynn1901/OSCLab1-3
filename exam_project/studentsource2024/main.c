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
} connmgrdata;

int MAX_CONN;
int PORT;
extern int conn_counter;

int main(int argc, char *argv[]) {
    //Configure the sensor number;
    if(argc < 3) {
        printf("Please provide the right arguments: first the port, then the max nb of clients");
        return -1;
    }

    MAX_CONN = atoi(argv[2]);
    PORT = atoi(argv[1]);

    pthread_t tid[3];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    connmgrdata connmgrdata = {MAX_CONN,PORT};
	pthread_create(&tid[0],&attr,run_connmgr,(void*)&connmgrdata);

    printf("Wait for thread close: Connection Magnager\n");
    pthread_join(tid[0],NULL);

    printf("Close all threads\n");

    return 0;
}
