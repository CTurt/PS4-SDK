#pragma once

#include "types.h"

typedef struct timeval SceKernelTimeval;

extern unsigned int (*sceKernelSleep)(unsigned int seconds);
extern int (*sceKernelUsleep)(unsigned int microseconds);
extern int (*sceKernelGettimeofday)(SceKernelTimeval *tp);
extern uint64_t (*sceKernelGetProcessTime)(void);
extern int (*sceKernelGetCurrentCpu)(void);

void initKernel(void);
