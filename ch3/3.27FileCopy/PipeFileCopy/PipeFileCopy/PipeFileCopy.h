#pragma once
#include "pch.h"

STARTUPINFO get_startup_info(HANDLE write_handle,HANDLE read_handle);

PROCESS_INFORMATION get_process(LPSTARTUPINFO startup_info, char* filename);
