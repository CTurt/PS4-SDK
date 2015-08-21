#include <stdlib.h>

#include "kernel.h"

#define DATA (void *)0x926300000

int _main(void) {
	// Load modules
	int libc;
	char libcName[] = "libSceLibcInternal.sprx";
	loadModule(libcName, &libc);
	
	
	// Resolve functions
	char *(*strcpy)(char *destination, const char *source);
	RESOLVE(libc, strcpy);
	
	
	// Copy some data
	char hello[] = "Hello!";
	strcpy(DATA, hello);
	
	
	// Use getpid system call
	int pid = syscall(20);
	
	
	// Return to browser
	return pid;
}
