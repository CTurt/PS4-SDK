#include "syscall.h"

#include "semaphore.h"

SYSCALL(createSemaphore, 549);
SYSCALL(removeSemaphore, 550);
SYSCALL(openSemaphore, 551);
SYSCALL(closeSemaphore, 552);
SYSCALL(waitSemaphore, 553);
SYSCALL(pollSemaphore, 554);
SYSCALL(signalSemaphore, 555);
SYSCALL(cancelSemaphore, 556);
