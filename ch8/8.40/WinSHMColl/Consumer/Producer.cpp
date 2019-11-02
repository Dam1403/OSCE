// Consumer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>

int* gen_collatz(int start, int* out_size);


int main( int argc, const char* argv[])
{
   

	if (argc != 2) {
		printf("Usage: Producer.exe <collats_start_int>");
		return 0;
	}

	int collatz_start = atoi(argv[1]);

	int res_size = 0;
	int* result = gen_collatz(collatz_start, &res_size);

	printf("%d",res_size);
	for (int i = 0; i < res_size; i++)
	{
		printf("%d ", result[i]);
	}
 
	
	HANDLE map_file_handle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 65536, L"CollatzMem");

	if (map_file_handle == NULL) {
		printf("map handle Null ");
		return 0;
	}

	int* mem = (int *) MapViewOfFile(map_file_handle, FILE_MAP_WRITE, 0, 0, 0);

	if (mem == NULL) {
		printf("Map View Failed");
		CloseHandle(map_file_handle);
		return  0;
	}
	mem[0] = res_size;
	
	for (int i = 0; i < res_size; i++)
	{
		mem[i + 1] = result[i];
	}
	getchar();
	UnmapViewOfFile(mem);
	CloseHandle(map_file_handle);
		
}


int* gen_collatz(int start, int* out_size)
{
	int seq[1024] = {0};

	int curr = start;
	int index = 0;
	while (curr != 1 && index < 1023)
	{
		seq[index] = curr;
		if (curr % 2 == 0)
		{
			curr = curr / 2;
		}
		else curr = (3 * curr) + 1;		
		index += 1;
	}
	seq[index] = 1;

	*out_size = index + 1;
	return seq;
	
	





	return seq;

}


