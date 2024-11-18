#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 50
#define READ_END 0
#define WRITE_END 1

int main(void){
    printf("Hello from %s!\n","SimpleLinuxFork");
    char wmsg[SIZE] = "What a Good Day";
    char rmsg[SIZE];
    pid_t pid;
    int fd[2];
    //create the pipe
    if(pipe(fd)==-1)
    {
        printf("Pipe failed\n");
        return 1;
    }

    pid = fork();

    if(pid<0){
        fprintf(stderr,"Forl failed");
    }
    if(pid>0)//parent process
    {
        close(fd[READ_END]);
        write(fd[WRITE_END],wmsg,strlen(wmsg)+1);
        close(fd[WRITE_END]);
    }
    else//child process
    {
        close(fd[WRITE_END]);
        read(fd[READ_END],rmsg,SIZE);
        printf("Child read %s\n",rmsg);
        for(int i=0;rmsg[i]!='\0';i++)
        {
            if(islower(rmsg[i]))
            {
                rmsg[i] = toupper(rmsg[i]);
            }
            else
            {
                rmsg[i] = tolower(rmsg[i]);
            }
        }
        printf("After convert is %s\n",rmsg);
        close(fd[READ_END]);
    }
    return 0;
}
