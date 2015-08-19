#include "syscall.h"

void _start(void) {
	// Pass address of a syscall gadget in rcx
	register f rcx asm("rcx");
	directSyscall = rcx;
	int _main(void);
	_main();
}
