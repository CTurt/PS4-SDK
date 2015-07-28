#pragma once

int createSemaphore(const char *name, int attributes, int startingCount, int maxCount);
int removeSemaphore(int semaphore);
