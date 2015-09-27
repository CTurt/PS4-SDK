#pragma once

#include "types.h"

#define MAP_SHARED 1
#define MAP_PRIVATE 2
#define MAP_TYPE 0xf
#define MAP_FIXED 0x10
#define MAP_ANONYMOUS 0x1000

struct memoryRegionInfo {
	void *base; // 0x0
	void *end; // 0x8
	unsigned int flags; // 0x16
};

struct otherMemoryRegionInfo {
	void *base; // 0x0
	void *end; // 0x8
	char unknown[0xa]; // 0x16
	char name[32]; // 0x20
};

void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
int munmap(void *addr, size_t len);

int getMemoryInfo(void *address, struct memoryRegionInfo *destination);
int getOtherMemoryInfo(void *address, int nextMatchIfUnmapped, struct otherMemoryRegionInfo *destination);
