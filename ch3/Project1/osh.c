

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "osh.h"

#define MAX_LINE 80



int main(void)
{
    char buff[MAX_LINE];
    char *args[MAX_LINE/2 + 1];
    int should_run = 1;

    while(should_run)
    {
        printf("osh>");
        fflush(stdout);
        
        
        fgets(buff,MAX_LINE,stdin);

        if(strnlen(buff,MAX_LINE) >= MAX_LINE - 1)
        {
            printf("Exceeded the maximum command length\n");
            return 0;    
        }

        char* cmd_chunk = strtok(buff," ");
        if(cmd_chunk[0] == '\n')
        {
            continue;
        }
        for(int i = 0; cmd_chunk != NULL; i++)
        {
            //printf("dsada");
            printf("Arg %s\n",cmd_chunk);
            args[i] = cmd_chunk;
            cmd_chunk = strtok(NULL," ");
        }
        printf("Arg %s\n",cmd_chunk);
        execute(args);

    }

    return 0;
}


void execute(char* cmd_chunks[])
{
    printf("Forking\n");
    int pid = fork();
    if(pid == 0)
    {
        printf("Exec Sucess %d\n",execvp(cmd_chunks[0],&cmd_chunks[1]));
    }
    else
    {
        wait(pid);
    }
}