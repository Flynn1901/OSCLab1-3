//
// Created by flynn on 24-11-28.
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>


#define NUM_THREADS 5
int sum = 0;

void *test_thread(void * arg){
    int counter = 0;
    while(counter<10){
        counter++;
        int mysum = sum;
        int r = rand()%2;
        sleep(1+r);
        sum = mysum+1;
        pid_t tid = syscall(SYS_gettid);
        printf("Thread = %ld = Counter = %d - sum = %d\n", tid, counter, sum);
    }
    // sleep(1);
    pthread_exit(0);
}

int main(int argc,char *argv[]){
    int i;
    pthread_t tid[NUM_THREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    for(i=0;i<NUM_THREADS;i++){
        pthread_create(&tid[i],&attr,test_thread,NULL);
    }
    for(i=0;i<NUM_THREADS;i++){
        pthread_join(tid[i],NULL);
    }
}