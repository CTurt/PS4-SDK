#pragma once

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

int getMemoryInfo(void *address, struct memoryRegionInfo *destination);
int getOtherMemoryInfo(void *address, struct otherMemoryRegionInfo *destination);
