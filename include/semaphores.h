#pragma once

int createSemaphore(const char *name, int attributes, int startingCount, int maxCount);
int removeSemaphore(int semaphore);
int waitSemaphore(int semaphore, int requiredCount, int *microsecondTimeout);
int pollSemaphore(int semaphore, int requiredCount);
int signalSemaphore(int semaphore, int count);
int cancelSemaphore(int semaphore, int count, int *threadsReleased);
