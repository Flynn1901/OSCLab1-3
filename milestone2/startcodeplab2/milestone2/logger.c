#include "logger.h"
#define SIZE 50

#define LOG_FILE "gateway.log"
static int sequency = 0;
FILE* log_file=NULL;

int create_log_process(void){
	log_file = fopen("gateway.log", "a");
	return 1;
}

int write_to_log_process(char *msg){
	sequency++;
	usleep(1000);
	create_log_process();
    if (!log_file)
    {
	    perror("Failed to open log file");
    	return 0;
    }
    time_t now = time(NULL);
	char time_str[SIZE];
	snprintf(time_str, SIZE, ctime(&now));

	int len = strlen(time_str);
	if (len>0)
	{
		time_str[len-1] = '\0';
	}

    fprintf(log_file, "%d-%s-%s\n", sequency,time_str,msg);
	end_log_process();
	return 1;
}

int end_log_process(void){
	fclose(log_file);
	return 0;
}