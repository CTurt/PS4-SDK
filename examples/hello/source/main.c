#include "kernel.h"
#include "libc.h"

int _main(void) {
	// Init and resolve libraries
	initLibc();


	// Copy some data
	strcpy(DATA, "Hello!");


	// Use getpid system call
	int pid = syscall(20);


	// Return to browser
	return pid;
}
