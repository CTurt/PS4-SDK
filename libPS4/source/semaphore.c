#include "syscall.h"

#include "semaphore.h"

int createSemaphore(const char *name, int attributes, int startingCount, int maxCount) {
	return syscall(549, name, attributes, startingCount, maxCount, 0);
}

SYSCALL(removeSemaphore, 550);
SYSCALL(openSemaphore, 551);
SYSCALL(closeSemaphore, 552);
SYSCALL(waitSemaphore, 553);
SYSCALL(pollSemaphore, 554);
SYSCALL(signalSemaphore, 555);
SYSCALL(cancelSemaphore, 556);
