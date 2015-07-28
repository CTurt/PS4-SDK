#include "syscall.h"

#include "semaphores.h"

int createSemaphore(const char *name, int attributes, int startingCount, int maxCount) {
	return syscall(549, name, attributes, startingCount, maxCount, 0);
}

int removeSemaphore(int semaphore) {
	return syscall(550, semaphore);
}

int waitSemaphore(int semaphore, int requiredCount, int *microsecondTimeout) {
	return syscall(553, semaphore, requiredCount, microsecondTimeout);
}

int pollSemaphore(int semaphore, int requiredCount) {
	return syscall(554, semaphore, requiredCount);
}

int signalSemaphore(int semaphore, int count) {
	return syscall(555, semaphore, count);
}

int cancelSemaphore(int semaphore, int count, int *threadsReleased) {
	return syscall(556, semaphore, count, threadsReleased);
}
