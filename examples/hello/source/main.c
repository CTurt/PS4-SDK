#include "kernel.h"
#include "libc.h"

int _main(void) {
	// Pass address of a syscall gadget in rcx
	register f rcx asm("rcx");
	directSyscall = rcx;
	
	
	// Init and resolve libraries
	initLibc();
	
	
	// Copy some data
	strcpy(DATA, "Hello!");
	
	
	// Use getpid system call
	int pid = syscall(20);
	
	
	// Return to browser
	return pid;
}
