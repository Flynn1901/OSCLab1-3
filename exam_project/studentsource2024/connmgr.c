//
// Created by flynn on 24-12-24.
//
#include "connmgr.h"
int conn_counter = 0;

typedef struct
{
    tcpsock_t *client;
    tcpsock_t *server;
    sbuffer_t *sbuffer;
} threadData;

typedef struct
{
    int MAX_CONN;
    int PORT;
    sbuffer_t *sbuffer;
} connmgrdata;

void *read_thread(void* arg)
{
    printf("Incoming client connection\n");
    printf("read_thread: conn_counter = %d\n", conn_counter);
    threadData* tdata = (threadData*)arg;
    tcpsock_t *client = tdata->client;
    tcpsock_t *server = tdata->server;
    sbuffer_t *sbuffer = tdata->sbuffer;
    sensor_data_t data;
    int bytes, result;
    do {
        // read sensor ID
        bytes = sizeof(data.id);
        result = tcp_receive(client, (void *) &data.id, &bytes);
        // read temperature
        bytes = sizeof(data.value);
        result = tcp_receive(client, (void *) &data.value, &bytes);
        // read timestamp
        bytes = sizeof(data.ts);
        result = tcp_receive(client, (void *) &data.ts, &bytes);
        if ((result == TCP_NO_ERROR) && bytes) {
            printf("sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", data.id, data.value,
                   (long int) data.ts);
            sensor_data_t *new_data = (sensor_data_t *)malloc(sizeof(sensor_data_t));
    		new_data->id = data.id;
    		new_data->ts = (long int) data.ts;
    		new_data->value = data.value;
            new_data->canberemoved = 0;
    		printf("Insert final data: %s \n",!sbuffer_insert(sbuffer,new_data)? "Success":"Fail");
        }
    } while (result == TCP_NO_ERROR);
    if (result == TCP_CONNECTION_CLOSED)
        printf("Peer has closed connection\n");
    else
        printf("Error occured on connection to peer\n");
    tcp_close(&client);
}

void *run_connmgr(void *arg){
    printf("Connection manager is started\n");
    tcpsock_t *server, *client;
    connmgrdata* Data = (connmgrdata*)arg;
    int MAX_CONN = Data->MAX_CONN;
    int PORT = Data->PORT;
    sbuffer_t *sbuffer = Data->sbuffer;

    pthread_t tid[MAX_CONN];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    printf("Test server is started\n");
    if (tcp_passive_open(&server, PORT) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    do {
        if (tcp_wait_for_connection(server, &client) != TCP_NO_ERROR) exit(EXIT_FAILURE);
        threadData data = {client,server,sbuffer};
        pthread_create(&tid[conn_counter],&attr,read_thread,(void*)&data);
        conn_counter++;
    } while (conn_counter < MAX_CONN);

    for (int i = 0; i < conn_counter; i++)
    {
        printf("Wait for thread close: thread %d\n",i);
        pthread_join(tid[i],NULL);
    }
    if (tcp_close(&server) != TCP_NO_ERROR) exit(EXIT_FAILURE);

    printf("Test server is shutting down\n");
}