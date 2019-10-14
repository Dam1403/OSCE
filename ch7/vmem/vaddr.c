#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    int mask = 0xFFF;
    int addr = atoi(argv[1]);

    printf("The address %d contains:\n",addr);
    printf("page number = %d\n", ((~mask) & addr) >> 12);
    printf("offset = %d\n", (mask) & addr);

}
