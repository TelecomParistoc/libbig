#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>


int main(int argc, char* argv[])
{
    if(argc<2)
    {
        fprintf(stdout,"Usage : %s <path to program>\n",argv[0]);
        return 1;
    }

    FILE* f = NULL;
    f = fopen("/var/log/log_robot/main.txt","a");
    if(!f)
    {
        fprintf(stderr,"/var/log/log_robot/main.txt has not been found\n");
        return 1;
    }

    time_t rawtime;
    struct tm *info;
    char buffer[80];

    int status;

    while(1)
    {
        time(&rawtime);
        info = localtime(&rawtime);
        strftime(buffer,80,"%c", info);

        int pid = fork();
        if(pid<0)
        {
            fprintf(stderr,"Error forking\n");
            return -1;
        }
        else if(pid)
        {
            fprintf(f, "Starting another instance at %s\n", buffer);
            fflush(f);
            wait(&status);

            time(&rawtime);
            info = localtime(&rawtime);
            strftime(buffer,80,"%c", info);
            fprintf(f, "Ending last instance at %s with status %d\n", buffer, status);
            fflush(f);
            sleep(1);
        }
        else
        {
            char path[100];
            path[0] = 0;
            strcat(path,"/var/log/log_robot/");
            strcat(path,buffer);
            printf("Redirecting stdout to %s\n",path);
            int fd = open(path,O_WRONLY|O_CREAT,0400);
            close(STDOUT_FILENO);
            dup2(fd,STDOUT_FILENO);
            if(execl(argv[1],argv[1],NULL)<0)
            {
                fprintf(stderr,"Error exec\n");
                return -1;
            }
        }
    }
}
