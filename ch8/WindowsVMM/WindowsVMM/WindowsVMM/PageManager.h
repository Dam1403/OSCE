#pragma once
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <fstream>



typedef struct pm_stats
{
	int hits;
	int misses;
}pm_stats;

typedef struct tlb_entry
{
	byte page_num;
	byte frame_num;
} tlb_entry;


typedef struct tlb_table {
	tlb_entry entries[16];
} tlb_table;

class PageManager {

private:
	HANDLE hdd_file_handle;
	HANDLE hdd_file_mapping;
	byte* hdd_view;
	tlb_table* TLB;
	pm_stats* stats;
	int oldest_frame = 0;
	int num_mask = 0x0000FF00;
	int off_mask = 0x000000FF;

	void add_to_tlb(byte page_num,byte frame_num)
	{
		tlb_entry* entry = &TLB->entries[oldest_frame];
		entry->page_num = page_num;
		entry->frame_num = frame_num;
		oldest_frame = (oldest_frame + 1) % 16;
	}

	byte get_frame_from_page_table(byte page_number)
	{
		/*
			I get this. make this if bored. 
		*/
		return page_number;
	}

public:
	PageManager()
	{
		printf("Stuck");
		hdd_file_handle = CreateFile(
			L"BACKING_STORE.bin",
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		printf("Stuck");
		if (hdd_file_handle == INVALID_HANDLE_VALUE)
		{
			printf("File Handle Null ");
			printf("%d", GetLastError());
			exit(1);
		}
		hdd_file_mapping = CreateFileMapping(
			hdd_file_handle,
			NULL, PAGE_READONLY,
			0, 65536,
			L"Backing_Store_Bin");

		TLB = (tlb_table*)malloc(sizeof(tlb_table) * 16);
		ZeroMemory(TLB, sizeof(tlb_table) * 16);
		stats = (pm_stats*)malloc(sizeof(pm_stats));
		ZeroMemory(stats, sizeof(pm_stats));
		
		if (TLB == NULL || stats == NULL) 
		{
			printf("tamagachi OS out of memory");
			exit(0);
		}

		stats->hits = 0;
		stats->misses = 0;
		if (hdd_file_mapping == NULL) {
			printf("map handle Null ");
			printf("%d", GetLastError());
			exit(1);
		}
		printf("Stuck");
		hdd_view = (byte*)MapViewOfFile(hdd_file_mapping, FILE_MAP_READ, 0, 0, 0);

		if (hdd_view == NULL) {
			printf("Map View Failure");
			printf("%d", GetLastError());
			CloseHandle(hdd_file_handle);
			exit(1);
		}

		printf("Stuck");
	}

	~PageManager()
	{
		UnmapViewOfFile(hdd_view);
		CloseHandle(hdd_file_mapping);
		CloseHandle(hdd_file_handle);
		free(TLB);
		free(stats);

	}

	byte read_addr(int addr) {
		int page_num = (addr & num_mask) >> 8;
		int offset = addr & off_mask;

		//Check TLB
		for (int i = 0; i < 16; i++)
		{
			//null entries arent checked but the memory
			//is zeroed so it'd work out in the end. 
			if (page_num == TLB->entries[i].page_num)
			{
				stats->hits += 1;
				int frame_index = TLB->entries[i].frame_num * 256;
				return hdd_view[frame_index + offset];
			}
			stats->misses += 1;
		}

		add_to_tlb(page_num, page_num);
		return  hdd_view[(256 * page_num) + offset] ;


	}

	pm_stats get_stats()
	{
		return *stats;
	}


};