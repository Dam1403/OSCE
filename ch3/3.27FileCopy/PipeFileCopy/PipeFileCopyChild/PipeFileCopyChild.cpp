// PipeFileCopyChild.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>


using namespace std;

int main( int argc, char* argv[])
{
	fstream outfile;

	outfile.open("log.txt", ios::out | ios::app);
	outfile << "===================================================\n";

	HANDLE output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);

	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	wstring out_filepath = converter.from_bytes(string(argv[1]));


	HANDLE file_handle = CreateFile(
		out_filepath.c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);


	if (file_handle == INVALID_HANDLE_VALUE) 
	{
		outfile << "Invalid Handle\n"; 
		exit(0);
	}

	DWORD bytes_read = 1;
	DWORD bytes_written = 0;
	char buff[1024];
	bool success = true;
	while (bytes_read != 0) {
		success = ReadFile(
			input_handle,
			&buff,
			1024,
			&bytes_read,
			NULL
		);
		if (!success) {
			outfile << "Read Failed\n";
			outfile << GetLastError();
		}
		success = WriteFile(
			file_handle,
			&buff,
			bytes_read,
			&bytes_written,
			NULL
		);
		if (!success) {
			outfile << "Read Success\n";
		}
	}

	CloseHandle(file_handle);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
