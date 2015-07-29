#include "syscall.h"

#include "mutexes.h"

int mutexInit(const char *name, unsigned int attributes) {
	return syscall(557, name, attributes);
}

int mutexDestroy(int mutex) {
	return syscall(558, mutex);
}
