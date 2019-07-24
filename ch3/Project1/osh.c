

#include <stdio.h>
#include <unistd.h>

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
        char* arg;

    }

    return 0;
}