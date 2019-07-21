// PipeFileCopyChild.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "pch.h"


int main(int argc, char* argv[])
{

	if (argc != 3) {
		printf("You're doing it wrong: PipeFileCopy.exe <input filepath> <output filepath>");
		return 0;
	}




	char* in_filepath = argv[1];
	char* out_filepath = argv[2];

	printf("%s", in_filepath);
	HANDLE read_handle = 0;
	HANDLE write_handle = 0;

	SECURITY_ATTRIBUTES sec_attrs;
	sec_attrs.bInheritHandle = true;
	sec_attrs.lpSecurityDescriptor = NULL;
	sec_attrs.nLength = sizeof(sec_attrs);



	bool success = CreatePipe(
		&read_handle,
		&write_handle,
		&sec_attrs,
		0);
	if (!SetHandleInformation(write_handle, HANDLE_FLAG_INHERIT, 0)) {
		printf("SetHandleInfo Failed: %d",GetLastError());
	}

	if (!success) {
		printf("Create Pipe Failed Error Code: %d", GetLastError());
		return 0;
	}


	//CLOSE HANDLES AFTER PROCESS EXITS
	STARTUPINFO startup_info = get_startup_info(write_handle,read_handle);

	PROCESS_INFORMATION process_info = get_process(&startup_info,out_filepath);

	std::ifstream infile;
	std::ofstream out_to_pipe;


	infile.open(in_filepath, std::ios::out | std::ios::binary);
	char buff[1024];
	printf("?\n");
	while (!infile.eof()) {
		infile.read(buff,1024);
		printf("??\n");
		DWORD bytes_written = 0;
		WriteFile(
			write_handle,
			(LPCVOID)buff,
			infile.gcount(),
			&bytes_written,
			NULL
		);
		
	}
	printf("File written to stdin\n");



	CloseHandle(read_handle);
	CloseHandle(write_handle);
	CloseHandle(process_info.hProcess);
	CloseHandle(process_info.hThread);


}


STARTUPINFO get_startup_info(HANDLE write_handle,HANDLE read_handle ) {

	STARTUPINFO info;
	info.cb = sizeof(STARTUPINFO);
	info.lpReserved = NULL;
	info.lpDesktop = NULL;
	info.lpTitle = NULL;
	info.dwFlags = STARTF_USESTDHANDLES;
	info.hStdInput = read_handle;
	info.hStdOutput = write_handle;
	info.hStdError = write_handle;
	
	info.dwFillAttribute;
	info.cbReserved2 = 0;
	info.lpReserved2 = NULL;
	return info;
}


PROCESS_INFORMATION get_process(LPSTARTUPINFO startup_info, char* filename)
{

	PROCESS_INFORMATION process_info;
	std::wstring executable_path = L"C:\\Users\\dougl\\Documents\\GitHub\\OSCE\\ch3\\3.27FileCopy\\PipeFileCopy\\Debug\\PipeFileCopyChild.exe ";
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide_filename = converter.from_bytes(std::string(filename));
	executable_path += wide_filename;
	std::wcout << executable_path;

	wchar_t wchar_exe_path[1024];
	wcscpy_s(wchar_exe_path,1024, executable_path.c_str());

	bool success = CreateProcessW(
		NULL,
		wchar_exe_path,//lpCommandline
		NULL,//proc sec attrs
		NULL,//thread sec attrs
		true,//inherit handles
		NORMAL_PRIORITY_CLASS,//dwCreationFlags
		NULL,//lpEnvironment
		NULL,//lpCurrentDirectory
		startup_info,
		&process_info);


	if (!success) {
		printf("Create Process Failed Error Code: %d", GetLastError());
		CloseHandle(startup_info->hStdInput);
		CloseHandle(startup_info->hStdOutput);
		exit(1);
	}

	return process_info;
}
