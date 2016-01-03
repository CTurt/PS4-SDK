#pragma once

extern int (*sceKernelJitCreateSharedMemory)(int flags, size_t size, int protection, int *destinationHandle);
extern int (*sceKernelJitCreateAliasOfSharedMemory)(int handle, int protection, int *destinationHandle);
extern int (*sceKernelJitMapSharedMemory)(int handle, int protection, void **destination);

void initJIT(void);

void allocateJIT(size_t size, void **executableAddress, void **writableAddress);
