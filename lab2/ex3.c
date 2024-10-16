#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
		while(1){

			time_t seconds;
			seconds = time(NULL);
			//printf("自 1970-01-01 起的小时数 = %ld\n", seconds/3600);
			srand((unsigned) seconds);
			//printf("Random number is: %d",rand()%45);
			int temp = rand()%45-10;
			time_t timer;
			struct tm* Now;
			time(&timer);
			Now = localtime(&timer);
			printf("Temperature = %d @ %s",temp,asctime(Now));
			//printf("Current time is %s",asctime(Now));

			sleep(1);
		}
		return 0;
}
