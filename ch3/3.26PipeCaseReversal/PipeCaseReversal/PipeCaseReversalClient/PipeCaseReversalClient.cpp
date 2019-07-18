// PipeCaseReversalClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "../PipeCaseReversal/PCRSharedConsts.h"





std::string get_input_string() 
{
	std::string input;
	std::cout << "Enter String to Reverse: ";
	std::cin >> input;
	while (input.length() >= MAX_PIPE_REVLEN - 1)
	{
		std::cout << "String too Long Try again: ";
		std::cin >> input;
	}
	return input;
}
int main()
{

	std::string toReverse;


	HANDLE pipe_handle = CreateFile(
		PIPE_REV_NAME,
		GENERIC_READ | GENERIC_WRITE,
		0,              // no sharing 
		NULL,           // default security attributes
		OPEN_EXISTING,  // opens existing pipe 
		0,              // default attributes 
		NULL
	);




	toReverse = get_input_string();

	while (toReverse.compare("quit") != 0) {
		DWORD bytes_written = 0;
		char buff[MAX_PIPE_REVLEN];

		strncpy_s(buff, toReverse.c_str(), MAX_PIPE_REVLEN - 1);

		printf("Sending: %s\n", buff);
		WriteFile(
			pipe_handle,
			(LPVOID)buff,
			toReverse.length(),
			&bytes_written,
			NULL);
		printf("Waiting\n");
		DWORD bytes_read = 0;
		ReadFile(
			pipe_handle,
			(LPVOID)buff,
			toReverse.length(),
			&bytes_read,
			NULL);

		printf("Recieved: %s\n", buff);
		toReverse = get_input_string();

	}



	system("pause");
	return 0;
	

}

