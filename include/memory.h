#pragma once

struct memoryRegionInfo {
	void *base; // 0x0
	void *end; // 0x8
	unsigned int flags; // 0x16
};

int getMemoryInfo(void *address, struct memoryRegionInfo *destination);
