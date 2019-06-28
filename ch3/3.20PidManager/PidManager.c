#include "PidManager.h"

#define MIN_PID 100
#define MAX_PID 8000

const int NO_PID_SLOTS = MAX_PID - MIN_PID;
int MAP_SIZE_BYTES = 0;

char* pid_map;

int allocate_map(){

	
    MAP_SIZE_BYTES = (NO_PID_SLOTS + sizeof(char))/sizeof(char);
	pid_map = (char*)calloc(MAP_SIZE_BYTES,sizeof(char)); 
	if(pid_map == NULL){
		return -1;
	}		
	return 1;
}


int allocate_pid(){
	char curr_byte = 0;
	for(int i = MIN_PID; i < MAP_SIZE_BYTES; i++){
		if(pid_map[i - MIN_PID] == 0){
			pid_map[i - MIN_PID] = 1;
			return i;
		}		
	}
	return -1;
	
		
}



void release_pid(int pid){
	if(pid < MAX_PID || pid >= MIN_PID){
		pid_map[pid - MIN_PID] = 0;
		return;
	}		
	printf("invalid release pid\n");
}



 
