#include "module.h"
#include "syscall.h"

#include "kernel.h"

int libKernelHandle;

int (*sceKernelLoadStartModule)(const char *name, size_t argc, const void *argv, unsigned int flags, int, int);

int (*sceKernelAllocateDirectMemory)(off_t searchStart, off_t searchEnd, size_t length, size_t alignment, int type, off_t *physicalAddressDestination);
int (*sceKernelMapDirectMemory)(void **addr, size_t length, int protection, int flags, off_t start, size_t alignment);

unsigned int (*sceKernelSleep)(unsigned int seconds);
int (*sceKernelUsleep)(unsigned int microseconds);
int (*sceKernelGettimeofday)(SceKernelTimeval *tp);
uint64_t (*sceKernelGetProcessTime)(void);
int (*sceKernelGetCurrentCpu)(void);

int (*sysctl)(int *name, unsigned int namelen, char *oldval, size_t *oldlen, char *newval, size_t newlen);
int (*sysctlbyname)(char *name, char *oldval, size_t *oldlen, char *newval, size_t newlen);
int (*sysarch)(int type, void *arg);
int (*execve)(char *path, char *argv[], char *envp[]);

void *(*pthread_self)();
int (*pthread_setaffinity_np)(void *one, long unsigned int two, void *three);

int (*sceKernelCreateEqueue)(SceKernelEqueue *eq, const char *name);
int (*sceKernelDeleteEqueue)(SceKernelEqueue eq);
int (*sceKernelAddUserEvent)(SceKernelEqueue eq, int id);
int (*sceKernelAddReadEvent)(SceKernelEqueue eq, int fd, size_t size, void *udata);

int (*getuid)();
int (*getgid)();
int (*getpid)();

int (*setuid)(int uid);
int (*setgid)(int gid);
int (*setreuid)(int ruid, int euid);
int (*setregid)(int rgid, int egid);


SYSCALL(kill, 37);
SYSCALL(ioctl, 54);

void initKernel(void) {
	loadModule("libkernel.sprx", &libKernelHandle);
	
	RESOLVE(libKernelHandle, sceKernelLoadStartModule);
	
	RESOLVE(libKernelHandle, sceKernelAllocateDirectMemory);
	RESOLVE(libKernelHandle, sceKernelMapDirectMemory);
	
	RESOLVE(libKernelHandle, sceKernelSleep);
	RESOLVE(libKernelHandle, sceKernelUsleep);
	RESOLVE(libKernelHandle, sceKernelGettimeofday);
	RESOLVE(libKernelHandle, sceKernelGetProcessTime);
	RESOLVE(libKernelHandle, sceKernelGetCurrentCpu);

	RESOLVE(libKernelHandle, sysctl);
	RESOLVE(libKernelHandle, sysctlbyname);
	RESOLVE(libKernelHandle, sysarch);
	RESOLVE(libKernelHandle, execve);

	RESOLVE(libKernelHandle, pthread_self);
	RESOLVE(libKernelHandle, pthread_setaffinity_np);

	RESOLVE(libKernelHandle, sceKernelCreateEqueue);
	RESOLVE(libKernelHandle, sceKernelDeleteEqueue);
	RESOLVE(libKernelHandle, sceKernelAddUserEvent);
	RESOLVE(libKernelHandle, sceKernelAddReadEvent);

	RESOLVE(libKernelHandle, getuid);
	RESOLVE(libKernelHandle, getgid);
	RESOLVE(libKernelHandle, getpid);

	RESOLVE(libKernelHandle, setuid);
	RESOLVE(libKernelHandle, setgid);
	RESOLVE(libKernelHandle, setreuid);
	RESOLVE(libKernelHandle, setregid);
}
