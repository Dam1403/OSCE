// WinSHMColl.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <iostream>

int main()
{


	HANDLE map_file_handle = OpenFileMapping(FILE_MAP_READ, false, L"CollatzMem");

	if (map_file_handle == NULL) {
		printf("map handle Null ");
		return 0;
	}

	int*  mem = (int*)MapViewOfFile(map_file_handle, FILE_MAP_READ, 0, 0, 0);

	if (mem == NULL) {
		printf("Map View Failure");
		printf("%d" ,GetLastError());
		CloseHandle(map_file_handle);
		return 0;
	}

	int size = mem[0];

	for (int i = 0; i < size; i++)
	{
		printf("%d ", mem[i + 1]);
	}

	UnmapViewOfFile(mem);
	CloseHandle(map_file_handle);
	

	return 0;


}
