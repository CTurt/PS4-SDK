#include "module.h"

#include "kernel.h"

int (*sceKernelAllocateDirectMemory)(off_t searchStart, off_t searchEnd, size_t length, size_t alignment, int type, off_t *physicalAddressDestination);
int (*sceKernelMapDirectMemory)(void **addr, size_t length, int protection, int flags, off_t start, size_t alignment);

unsigned int (*sceKernelSleep)(unsigned int seconds);
int (*sceKernelUsleep)(unsigned int microseconds);
int (*sceKernelGettimeofday)(SceKernelTimeval *tp);
uint64_t (*sceKernelGetProcessTime)(void);
int (*sceKernelGetCurrentCpu)(void);

int kill(int pid, int signum) {
	return syscall(37, pid, signum);
}

void initKernel(void) {
	int libkernel;
	loadModule("libkernel.sprx", &libkernel);
	
	RESOLVE(libkernel, sceKernelAllocateDirectMemory);
	RESOLVE(libkernel, sceKernelMapDirectMemory);
	
	RESOLVE(libkernel, sceKernelSleep);
	RESOLVE(libkernel, sceKernelUsleep);
	RESOLVE(libkernel, sceKernelGettimeofday);
	RESOLVE(libkernel, sceKernelGetProcessTime);
	RESOLVE(libkernel, sceKernelGetCurrentCpu);
}
