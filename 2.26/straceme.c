#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h>

size_t MAX_FILENAME_LEN = 32;
size_t COPY_BUF_LEN = 64;

int get_file(char* filename){	
	int FD = open(filename,O_RDWR|O_CREAT);
	if(FD == -1){
		printf("ERROR");
		return;
	}
	return FD;
}



void copy(char* infilename, char* outfilename){
	char* copy_buf = malloc(COPY_BUF_LEN);
	int infile = get_file(infilename);
	int outfile = get_file(outfilename);
	
	int bytes_read = -1;
	do{
		bytes_read = read(infile,copy_buf,COPY_BUF_LEN);
		write(outfile,copy_buf,bytes_read);
		printf("%d",bytes_read);
	}while(bytes_read != 0);	
	free((void *)copy_buf);
	close(infile);
	close(outfile);
}



int main(){
	char* infilename = (char*) calloc(0,MAX_FILENAME_LEN);
	char*  outfilename = (char*) calloc(0,MAX_FILENAME_LEN);
	int newline_index = 0;
	printf("input filename:");	
	newline_index = getline(&infilename, &MAX_FILENAME_LEN,stdin) - 1;
	infilename[newline_index] = '\0';
	printf("output filename:");
	newline_index = getline(&outfilename, &MAX_FILENAME_LEN,stdin) - 1;
	outfilename[newline_index] = '\0';
	
	copy(infilename,outfilename);

	free((void *)infilename);
	free((void *)outfilename);
}

