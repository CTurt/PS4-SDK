#include "syscall.h"

#include "memory.h"

int getMemoryInfo(void *address, struct memoryRegionInfo *destination) {
	return syscall(547, address, destination);
}

int getOtherMemoryInfo(void *address, int nextMatchIfUnmapped, struct otherMemoryRegionInfo *destination) {
	return syscall(572, address, nextMatchIfUnmapped, destination, sizeof(struct otherMemoryRegionInfo));
}
