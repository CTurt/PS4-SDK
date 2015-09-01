#include "ps4.h"

void *thread_func(void *arg) {
	int *p = (int *)arg;

	*p = *p + 1;

	return NULL;
}

int _main(void) {
	// Init and resolve libraries
	initKernel();
	
	initLibc();
	initPthread();

	int foo = 0;

	ScePthread thread;
	scePthreadCreate(&thread, NULL, thread_func, (void *)&foo, "pthread_sample");

	// Wait until the thread terminates
	scePthreadJoin(thread, NULL);

	return foo;
}
