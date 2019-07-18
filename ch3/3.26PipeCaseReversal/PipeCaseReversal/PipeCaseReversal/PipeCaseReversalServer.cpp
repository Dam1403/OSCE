// PipeCaseReversal.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "PCRSharedConsts.h"
#include <iostream>


void cap_reverse(char* buff, int str_len)
{

	if (str_len > MAX_PIPE_REVLEN)
	{
		std::cout << "???";
		return;
	}

	for (int i = 0; i < str_len; i++)
	{
		if (isupper(buff[i]))
		{
			buff[i] = tolower(buff[i]);
		}
		else
		{
			buff[i] = toupper(buff[i]);
		}
	}
}


int main()
{
	HANDLE pipe_handle = CreateNamedPipe(
		PIPE_REV_NAME, //name
		PIPE_ACCESS_DUPLEX, //openMode
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, //Pipe Mode
		1,//Max Instances
		1024,//OutBuffSize
		1024,//InBuffSize
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	

	
	if (pipe_handle == INVALID_HANDLE_VALUE)
	{
		printf("Create Pipe Failure Error: %d\n",GetLastError());
		return 1;
	}

	std::cout << "Waiting For Client\n";
	bool connection = ConnectNamedPipe(pipe_handle, NULL);

	if (connection)
	{
		printf("Pipe Connection Successful\n");
	}
	else
	{
		printf("Pipe Connection Error: %d\n", GetLastError());
		return 1;
	}

	char string_to_reverse[MAX_PIPE_REVLEN];
	while (true) {
		
		DWORD bytes_read;
		ReadFile(
			pipe_handle,
			(LPVOID)string_to_reverse,
			MAX_PIPE_REVLEN - 1,
			&bytes_read,
			NULL);	
		string_to_reverse[bytes_read] = '\0';
		printf("Recieved: %s\n", string_to_reverse);
		cap_reverse(string_to_reverse, bytes_read);
		printf("Sending: %s\n", string_to_reverse);

		DWORD bytes_written;

		WriteFile(
			pipe_handle,
			(LPVOID)string_to_reverse,
			bytes_read,
			&bytes_written,
			NULL);
	}
	getchar();
	return 0;






}




