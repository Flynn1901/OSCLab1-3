#include "logger.h"
#define SIZE 100

#define LOG_FILE "gateway.log"

void write_to_log(const char *message) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (!log_file) {
        perror("Failed to open log file");
        return;
    }

    time_t now = time(NULL);
    fprintf(log_file, "%s - %s", ctime(&now), message);
    fclose(log_file);
}

int main() {
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), stdin)) {
        if (strcmp(buffer, "EXIT\n") == 0) break; // 收到退出信号
        printf("%s",buffer);
        write_to_log(buffer); // 写日志
    }

    printf("Logger exited.\n");
    return 0;
}