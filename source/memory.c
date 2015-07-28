#include "syscall.h"

#include "memory.h"

int getMemoryInfo(void *address, struct memoryRegionInfo *destination) {
	return syscall(547, address, destination);
}

int getOtherMemoryInfo(void *address, struct otherMemoryRegionInfo *destination) {
	return syscall(572, address, 0, destination, sizeof(struct otherMemoryRegionInfo));
}
