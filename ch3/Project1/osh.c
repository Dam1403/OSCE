

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "osh.h"

#define MAX_LINE 80
#define TRUE 0
#define FALSE 1



int main(void)
{
    char *args[MAX_LINE/2 + 1];
    char buff[MAX_LINE];

    int should_run = 1;

    while(should_run)
    {
        printf("osh>");
        fflush(stdout);
        
        
        fgets(buff,MAX_LINE,stdin);
        int buff_len = strnlen(buff,MAX_LINE);
        if(buff_len >= MAX_LINE - 1)
        {
            printf("Exceeded the maximum command length\n");
            return 0;    
        }

        if(buff[0] == '\n')
        {
            continue;
        }

        int run_in_background = FALSE;
        //"ls\n\0"
        // 01 2 3
        buff[buff_len - 1] = '\0';
        if(buff[buff_len - 2] == '&')
        {
            buff[buff_len - 2] = '\0';
            run_in_background = TRUE;
            printf("background\n");
        }

        parse_line(args,buff,buff_len);
        execute(args,run_in_background);

    }

    return 0;
}

void parse_line(char* args[],char* buff, int buff_len)
{
    int arg_index = 0;
    int nulling = TRUE;
    for(int i = 0; i < buff_len; i++)
    {

        if(buff[i] == ' ')
        {

            buff[i] = '\0';
            nulling = TRUE;
        }
        else if(nulling == TRUE)
        {
            nulling = FALSE;
            args[arg_index] = &buff[i];
            arg_index += 1;   
        }
    }
    args[arg_index] = NULL;
}

void execute(char* cmd_chunks[], int background)
{
    //printf("Forking cmd %s\n",cmd_chunks[0]);
    int pid = fork();
    if(pid == 0)
    {
        int exit_success = execvp(cmd_chunks[0],cmd_chunks);
        if(exit_success < 0)
        {
            printf("%s: Command Not Found\n", cmd_chunks[0]);
        }
    }
    else
    {
        if(background == FALSE)
        {
            waitpid(pid,NULL,0);
        }

    }
}