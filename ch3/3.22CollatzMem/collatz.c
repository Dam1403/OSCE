#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>

typedef struct CollResult{
	int size;
	int nums[256];
	
}CollResult;

void gen_collatz(int start_num)
{
	int sh_fd = shm_open("/Collatz", O_RDWR,S_IRUSR | S_IWUSR);
	if(sh_fd < 0){
		printf("Child SH open Failure: %s\n",strerror(errno));		
	}

	CollResult* result = (CollResult *) mmap(
		NULL,
		sizeof(CollResult),
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		sh_fd,
		0);
	int curr_num = start_num;
	for(int i = 0; (curr_num != 1) && (i < 255); i++)
	{
		if(curr_num % 2 == 0)
		{
			curr_num = curr_num / 2;
		}
		else
		{
			curr_num = (3 * curr_num) + 1;	
		}
		result->nums[i] = curr_num;
		result->size = i;	
	}	
	result->nums[result->size] = 1;
	result->size = result->size += 1;	
}


int main(){
	char number[10];
	int start_num = 0;

	printf("Enter Starting Number: ");
	fgets(number,10,stdin);
	start_num = atoi(number);
	//printf("%d\n",start_num);

	int sh_fd = shm_open("/Collatz",O_CREAT |  O_RDWR, S_IRUSR | S_IWUSR);	
	if(sh_fd < 0){
		printf("Parent SH open Failure");		
	}
	if(ftruncate(sh_fd,sizeof(CollResult)) == -1){
		printf("Truncate Failure");	
	}
	
	CollResult* result = (CollResult *) mmap(
		NULL,
		sizeof(CollResult),
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		sh_fd,
		0);
	
	int pid = fork();
	if(pid != 0){
		waitpid(pid,NULL,0);
		printf("size: %d\n",result->size);
		printf("Result: \n");
		for(int i = 0; i < result->size; i++)
		{
			printf("%d, ",result->nums[i]);
		}
		printf("\nMain Process Exiting :D\n");
	}
	else
	{
		gen_collatz(start_num);
	}
	shm_unlink("/Collatz");
	
}
