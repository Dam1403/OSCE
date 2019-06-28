#include <unistd.h>
#include <stdio.h>

int main(){
	int pid = fork();
	if(pid > 0){
		printf("Papa Process %i\n",getpid());
		fflush(stdout);
		sleep(30);
		
	}
	else{
		printf("Future Zombie Here %i\nLook for me in htop\n",getpid());
	}
}

