#ifndef PIDMAN
#define PIDMAN
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

 
int allocate_map();
int allocate_pid();
void release_pid(int pid);

#endif
