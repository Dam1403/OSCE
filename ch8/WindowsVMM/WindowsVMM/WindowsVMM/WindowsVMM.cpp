// WindowsVMM.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "PageManager.h"
int main()
{
	std::ifstream addresses;
	addresses.open("addrs.txt");
	int num_mask = 0x0000FF00;
	int off_mask = 0x000000FF;
	if (!addresses)
	{
		std::cout << "Couldnt Open Addresses\n";
		exit(1);
	}

	int addr = 0;
	PageManager pm;

	while (addresses >> addr)// ">>"? lmao 
	{
		int page_num = (addr & num_mask) >> 8;
		int offset = addr & off_mask;
	

		
		byte read = pm.read_addr(addr);
		if (read != page_num) {
			printf("Read %d page_num is %d",read, page_num);
			exit(1);
		}
		
	}
	pm_stats stats = pm.get_stats();
	printf("Hits: %d Misses %d pct: %f",stats.hits,stats.misses,(float)stats.hits/stats.misses);

	
	
}
