

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
#define SIZE 201
#define READ_END 0
#define WRITE_END 1

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

pid_t pid;
int fd[2];

int MAX_CONN;
int PORT;
sbuffer_t *sbuffer;
int complete_transfer=0;
pthread_mutex_t mutex;
pthread_mutex_t mutex_log;
FILE* log_file=NULL;
static int sequency = 0;

int write_to_log_process(char *msg){
    for (int i=0; i<SIZE; i++)
    {
        printf("%c", msg[i]);
    }
    printf("\n");
    log_file = fopen("gateway.log", "a");
    if (!log_file)
    {
        perror("Failed to open log file");
        return 0;
    }

    time_t now = time(NULL);
    char time_str[SIZE];
    snprintf(time_str, SIZE, "%s",ctime(&now));
    int len = strlen(time_str);
    if (len>0)
    {
        time_str[len-1] = '\0';
    }
    while (*msg != '\0') {
        size_t length = strlen(msg);
        fprintf(log_file, "%d - %s - %.*s\n",sequency++,time_str,(int)length+1, msg);
        fflush(log_file);
        msg += length + 1;
    }
    fclose(log_file);
    return 1;
}




int main(int argc, char *argv[]) {
    //Configure the sensor number;
    if(argc < 3) {
        printf("Please provide the right arguments: first the port, then the max nb of clients");
        return -1;
    }
    MAX_CONN = atoi(argv[2]);
    PORT = atoi(argv[1]);
    FILE * map = fopen("room_sensor.map", "r");

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex init failed");
        return 1;
    }

    if (pthread_mutex_init(&mutex_log, NULL) != 0) {
        perror("Mutex_Log init failed");
        return 1;
    }

    sbuffer= malloc(SBUFFER_SIZE);
    printf("%s",!sbuffer_init(&sbuffer)? "OK" : "ERROR");

    if (pipe(fd) == -1) {
        perror("Pipe failed");
        exit(1);
    }
    pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) { // 子进程
        printf("This is stupid child process\n");
        close(fd[WRITE_END]); // 关闭写端
        char buffer[SIZE];
        memset(buffer, 0, SIZE);

        while (read(fd[READ_END],buffer,SIZE-1))
        {
            write_to_log_process(buffer); // 写日志
            memset(buffer, 0, SIZE);
        }
        close(fd[READ_END]);
        exit(0);
    }

    else//parent process
    {
        printf("This is parent process\n")
;        pthread_t tid[3];
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        connmgrdata connmgrdata1 = {MAX_CONN,PORT,sbuffer};
        pthread_create(&tid[0],&attr,run_connmgr,(void*)&connmgrdata1);
        datamgrData datamgrdata = {map,sbuffer};
        pthread_create(&tid[1],&attr,datamgr_parse_sensor_files,(void*)&datamgrdata);
        stormgrData stormgrdata = {sbuffer};
        pthread_create(&tid[2],&attr,stormgr,(void*)&stormgrdata);


        printf("Wait for thread close: Connection Magnager\n");
        pthread_join(tid[0],NULL);
        pthread_join(tid[1],NULL);
        pthread_join(tid[2],NULL);

        printf("Close all threads\n");
    }



    return 0;
}
