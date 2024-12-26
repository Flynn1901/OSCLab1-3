//
// Created by flynn on 24-12-24.
//
#include "connmgr.h"
extern int complete_transfer;
extern pthread_mutex_t mutex;

typedef struct
{
    tcpsock_t *client;
    sbuffer_t *sbuffer;
} threadData;

typedef struct
{
    int MAX_CONN;
    int PORT;
    sbuffer_t *sbuffer;
    pthread_mutex_t* mutex;
} connmgrdata;

void *read_thread(void* arg)
{
    printf("Incoming client connection\n");
    threadData* tdata = (threadData*)arg;
    tcpsock_t *client = tdata->client;
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
            printf("Connection Manager: sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", data.id, data.value,
                   (long int) data.ts);
            sensor_data_t *new_data = (sensor_data_t *)malloc(sizeof(sensor_data_t));
            new_data->id = data.id;
            new_data->ts = (long int) data.ts;
            new_data->value = data.value;
            new_data->canberemoved = 0;
            pthread_mutex_lock(&mutex);
    		printf("Insert final data: %s \n",!sbuffer_insert(sbuffer,new_data)? "Success":"Fail");
            pthread_mutex_unlock(&mutex);
        }
    } while (result == TCP_NO_ERROR);
    if (result == TCP_CONNECTION_CLOSED)
        printf("Peer has closed connection\n");
    else
        printf("Error occured on connection to peer\n");
    tcp_close(&client);
    return NULL;
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
    int conn_counter1 = 0;
    while (conn_counter1 < MAX_CONN)
    {
        if (tcp_wait_for_connection(server, &client) != TCP_NO_ERROR) {
            fprintf(stderr,"Error: tcp_wait_for_connection() failed.\n");
            tcp_close(&server);
            exit(EXIT_FAILURE);
        }
        threadData* data = malloc(sizeof(threadData));
        if (!data) {
            fprintf(stderr, "Error: Failed to allocate memory for threadData.\n");
            tcp_close(&client);
            continue;
        }
        data->client = client;
        data->sbuffer = sbuffer;
        printf("read_thread: conn_counter = %d\n", conn_counter1);
        if (pthread_create(&tid[conn_counter1], NULL , read_thread, (void*)data) != 0) {
            perror("Failed to create thread");
            tcp_close(&client);
            free(data);
            continue;
        }
        conn_counter1++;
    }

    for (int i = 0; i < conn_counter1; i++)
    {
        printf("Wait for thread close: thread %d\n",i);
        pthread_join(tid[i],NULL);
        printf("Thread %d close",i);
    }
    if (tcp_close(&server) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    printf("Complete TCP transfer\n");
    complete_transfer = 1;

    printf("Test server is shutting down\n");

    return NULL;
}