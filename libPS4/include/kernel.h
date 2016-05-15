#pragma once

#include "types.h"

typedef struct timeval SceKernelTimeval;
typedef uint64_t SceKernelEqueue;

extern int libKernelHandle;

extern int (*sceKernelLoadStartModule)(const char *name, size_t argc, const void *argv, unsigned int flags, int, int);

extern int (*sceKernelAllocateDirectMemory)(off_t searchStart, off_t searchEnd, size_t length, size_t alignment, int type, off_t *physicalAddressDestination);
extern int (*sceKernelMapDirectMemory)(void **addr, size_t length, int protection, int flags, off_t start, size_t alignment);

extern unsigned int (*sceKernelSleep)(unsigned int seconds);
extern int (*sceKernelUsleep)(unsigned int microseconds);
extern int (*sceKernelGettimeofday)(SceKernelTimeval *tp);
extern uint64_t (*sceKernelGetProcessTime)(void);
extern int (*sceKernelGetCurrentCpu)(void);

extern int (*sysctl)(int *name, unsigned int namelen, char *oldval, size_t *oldlen, char *newval, size_t newlen);
extern int (*sysctlbyname)(char *name, char *oldval, size_t *oldlen, char *newval, size_t newlen);
extern int (*sysarch)(int type, void *arg);
extern int (*execve)(char *path, char *argv[], char *envp[]);
int ioctl(int fd, unsigned long com, void *data);

extern void *(*pthread_self)();
extern int (*pthread_setaffinity_np)(void *one, long unsigned int two, void *three);

extern int (*sceKernelCreateEqueue)(SceKernelEqueue *eq, const char *name);
extern int (*sceKernelDeleteEqueue)(SceKernelEqueue eq);
extern int (*sceKernelAddUserEvent)(SceKernelEqueue eq, int id);
extern int (*sceKernelAddReadEvent)(SceKernelEqueue eq, int fd, size_t size, void *udata);

extern int (*getuid)();
extern int (*getgid)();
extern int (*getpid)();

extern int (*setuid)(int uid);
extern int (*setgid)(int gid);
extern int (*setreuid)(int ruid, int euid);
extern int (*setregid)(int rgid, int egid);

int kill(int pid, int signum);

void initKernel(void);
