#include "syscall.h"

#include "barrier.h"

int barrierInit(const char *name, unsigned int attributes, int count) {
	return syscall(557, name, attributes, count);
}

int barrierDestroy(int barrier) {
	return syscall(558, barrier);
}
