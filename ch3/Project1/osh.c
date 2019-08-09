#include "osh.h"




int main(void)
{
    char *args[MAX_LINE/2 + 1];
    char buff[MAX_LINE];
    HistoryEntry* head = NULL;

    int should_run = 1;

    int history_counter = 1;

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
        }


        if(strncmp(args[0],"history",8) == 0)
        {
            show_history(head);
            continue;
        }
        add_to_history(&head,buff,buff_len);
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

void show_history(HistoryEntry* head)
{
    HistoryEntry* curr = head;
    if (head == NULL)
    {
        printf("No History \n");
        return;
    }
    for(int i = 0;curr != NULL && i < MAX_HIST;i++)
    {
        printf("%d %s\n",curr->id,curr->command);
        curr = curr->next;
    }
}

void add_to_history(HistoryEntry** head,char* buff,int buff_len)
{
    HistoryEntry* new_entry = create_history(buff,buff_len);

    if(*head == NULL)
    {
        (*head) = new_entry;
        (*head)->id = 1;
        return;
    }

    new_entry->next = *head;
    new_entry->id = (*head)->id + 1;
    *head = new_entry;

}

HistoryEntry* create_history(char* buff,int buff_len)
{
    HistoryEntry* new_entry = (HistoryEntry*)malloc(sizeof(HistoryEntry));
    
    memcpy(new_entry->command,buff,buff_len);
    new_entry->next = NULL;

    return new_entry;
}