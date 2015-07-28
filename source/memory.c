#include "syscall.h"

#include "memory.h"

int getMemoryInfo(void *address, struct memoryRegionInfo *destination) {
	return syscall(547, address, destination);
}
