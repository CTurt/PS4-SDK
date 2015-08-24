#include "module.h"

#include "kernel.h"

unsigned int (*sceKernelSleep)(unsigned int seconds);
int (*sceKernelUsleep)(unsigned int microseconds);
int (*sceKernelGettimeofday)(SceKernelTimeval *tp);
uint64_t (*sceKernelGetProcessTime)(void);
int (*sceKernelGetCurrentCpu)(void);

void initKernel(void)
{
	int libkernel;
	loadModule("libkernel.sprx", &libkernel);

	RESOLVE(libkernel, sceKernelSleep);
	RESOLVE(libkernel, sceKernelUsleep);
	RESOLVE(libkernel, sceKernelGettimeofday);
	RESOLVE(libkernel, sceKernelGetProcessTime);
	RESOLVE(libkernel, sceKernelGetCurrentCpu);
}
