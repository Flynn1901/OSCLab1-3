#include "sensor_db.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 201
#define READ_END 0
#define WRITE_END 1

static pid_t pid;
static int fd[2];

FILE* open_db(char *file, bool append)
{
    printf("Hello from %s!\n", "SimpleLinuxFork");

    // 创建管道
    if (pipe(fd) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    // 创建子进程
    pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }
    if (pid == 0) { // 子进程
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
    else
    {
        // 父进程
        char *mode = append ? "a" : "w";
        FILE *db = fopen(file, mode);
        if (db == NULL) {
            perror("Failed to open file");
            return NULL;
        }

        // 向 Logger 发送日志
        char message1[SIZE];
        snprintf(message1, SIZE, "Data file opened.");
        write(fd[WRITE_END], message1, strlen(message1)+1);
        return db;
    }
}

int insert_sensor(FILE *f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
    if (f == NULL) return -1;

    // 写入数据到文件
    fprintf(f, "%d, %f, %ld\n", id,value,ts);

    // 发送日志消息到 Logger
    char message2[SIZE];
    snprintf(message2, SIZE, "Data inserted.");
    write(fd[WRITE_END], message2, strlen(message2)+1);
    return 0;
}

int close_db(FILE *f){
    if (f) fclose(f);

    // 发送退出消息给 Logger
    if (pid > 0)
    {
        char message3[SIZE];
        snprintf(message3, SIZE, "Data file closed.");
        write(fd[WRITE_END], message3, strlen(message3)+1);
        close(fd[WRITE_END]); // 关闭管道写端
    }
    waitpid(pid, NULL, 0);
    return 0;
}