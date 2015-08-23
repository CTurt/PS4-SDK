#include "kernel.h"

#include "libc.h"
#include "pthread.h"

void *thread_func(void *arg)
{
	int *p = (int *)arg;

	*p = *p + 1;

	return NULL;
}

int _main(void)
{
	// Pass address of a syscall gadget in rcx
	register f rcx asm("rcx");
	directSyscall = rcx;

	// Init and resolve libraries
	initLibc();
	initPthread();

	int foo = 0;

	ScePthread thread;
	scePthreadCreate(&thread, NULL, thread_func, (void *)&foo, "pthread_sample");

	// Wait until the thread terminates
	scePthreadJoin(thread, NULL);

	return foo;
}
