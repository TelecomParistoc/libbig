#include <unistd.h>
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
    f = fopen("/var/log/log.txt","a");
    if(!f)
    {
        fprintf(stderr,"log.txt has not been found\n");
        return 1;
    }

    time_t rawtime;
    struct tm *info;
    char buffer[80];

    int *status;

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
            fprintf(f, "Starting another instance at %s\n", buf);
            wait(&status);

            time(&rawtime);
            info = localtime(&rawtime);
            strftime(buffer,80,"%c", info);
            fprintf(f, "Ending last instance at %s with status %d\n", buf, *status);
        }
        else
        {
            char path[100];
            path[0] = 0;
            strcat(path,"/var/log/");
            strcat(path,buffer);
            int fd = open(path,O_WRONLY|O_CREAT,0o400);
            close(stdout);
            dup2(stdout, fd);
            execl(argv[1],argv[1],NULL);
        }
    }
}
