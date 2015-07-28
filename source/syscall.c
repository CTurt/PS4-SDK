#include "syscall.h"

unsigned long syscall(unsigned long n, ...) {
	register unsigned long rax asm("rax");
	register unsigned long rcx asm("rcx");
	register unsigned long r10 asm("r10");
	
	r10 = rcx;
	(void)r10;
	rax = 0;
	asm("syscall");
	return rax;
}
