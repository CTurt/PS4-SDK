#include "syscall.h"

#include "semaphores.h"

int createSemaphore(const char *name, int attributes, int startingCount, int maxCount) {
	return syscall(549, name, attributes, startingCount, maxCount, 0);
}

int removeSemaphore(int semaphore) {
	return syscall(550, semaphore);
}
