#include "sensor_db.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 50
#define READ_END 0
#define WRITE_END 1

static pid_t pid;
static int sequency = 0;
int fd[2];

FILE* open_db(char *file, bool append) {
    printf("Hello from %s!\n", "SimpleLinuxFork");


    // 创建管道
    if (pipe(fd) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    // 创建子进程
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) { // 子进程
        close(fd[1]); // 关闭写端
        dup2(fd[0], STDIN_FILENO); // 重定向管道读端到标准输入
        close(fd[0]); // 关闭重定向后的描述符
        execl("./logger", "Logger", NULL); // 启动 logger 程序
        perror("execl failed"); // 如果 execl 失败
        exit(1);
    } else { // 父进程
        char *mode = append ? "a" : "w";
        FILE *db = fopen(file, mode);
        if (db == NULL) {
            perror("Failed to open file");
            return NULL;
        }

        // 向 Logger 发送日志
        char message[256];
        snprintf(message, sizeof(message), "Opened file: %s\n", file);
        write(fd[WRITE_END], message, strlen(message));

        return db;
    }
}

int insert_sensor(FILE *f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
    if (f == NULL) return -1;

    // 写入数据到文件
    fprintf(f, "%s\n", "test");
    fflush(f);

    // 发送日志消息到 Logger
    char message[256];
    snprintf(message, sizeof(message), "Inserted data: %s\n", "Close to answer");
    write(fd[WRITE_END], message, strlen(message));
    return 0;
}

int close_db(FILE *f){
    if (f) fclose(f);

    // 发送退出消息给 Logger
    write(fd[WRITE_END], "EXIT\n", 5);
    close(fd[WRITE_END]); // 关闭管道写端
    waitpid(pid, NULL, 0); // 等待子进程结束
    return 0;
}