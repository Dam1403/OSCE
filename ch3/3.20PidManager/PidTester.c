#include "PidManager.h"


void allocate_all(){
	for(int i = 0; i < 7901; i++){
		int pid = allocate_pid();
		printf("Recieved PID: %i\n",pid);
	}
}
void release_all(){

	for(int i = 0; i < 7901; i++){
		release_pid(i);
		printf("Released PID: %i\n",i);
	}
}


int main(){
	srand(1);
	if(allocate_map() == -1){
		printf("Allocation Failure\n");
		exit(0);
	}
	
	allocate_all();
	release_all();
	allocate_all();
	release_pid(799);
	int pid = allocate_pid();

	printf("Recieved PID: %i\n",pid); 	
}
