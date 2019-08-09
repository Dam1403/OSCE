


#define MAX_LINE 80
#define MAX_HIST 10
#define TRUE 0
#define FALSE 1


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>



typedef struct HistoryEntry{
    int id;
    char command [MAX_LINE];
    struct HistoryEntry* next;
} HistoryEntry;

void execute(char* cmd_chunks[], int background);

void parse_line(char* args[],char* buff, int buff_len);

void show_history(HistoryEntry* head);

void add_to_history(HistoryEntry** head, char* buff,int buff_len);

HistoryEntry* create_history(char* buff,int buff_len);

void get_history(int comm_index); 
