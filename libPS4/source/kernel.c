#include "module.h"
#include "syscall.h"

#include "kernel.h"

int (*sceKernelLoadStartModule)(const char *name, size_t argc, const void *argv, unsigned int flags, int, int);

int (*sceKernelAllocateDirectMemory)(off_t searchStart, off_t searchEnd, size_t length, size_t alignment, int type, off_t *physicalAddressDestination);
int (*sceKernelMapDirectMemory)(void **addr, size_t length, int protection, int flags, off_t start, size_t alignment);

unsigned int (*sceKernelSleep)(unsigned int seconds);
int (*sceKernelUsleep)(unsigned int microseconds);
int (*sceKernelGettimeofday)(SceKernelTimeval *tp);
uint64_t (*sceKernelGetProcessTime)(void);
int (*sceKernelGetCurrentCpu)(void);

int (*sysctl)(int *name, unsigned int namelen, char *oldval, size_t *oldlen, char *newval, size_t *newlen);
int (*sysctlbyname)(char *name, char *oldval, size_t *oldlen, char *newval, size_t *newlen);
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
	int libkernel;
	loadModule("libkernel.sprx", &libkernel);
	
	RESOLVE(libkernel, sceKernelLoadStartModule);
	
	RESOLVE(libkernel, sceKernelAllocateDirectMemory);
	RESOLVE(libkernel, sceKernelMapDirectMemory);
	
	RESOLVE(libkernel, sceKernelSleep);
	RESOLVE(libkernel, sceKernelUsleep);
	RESOLVE(libkernel, sceKernelGettimeofday);
	RESOLVE(libkernel, sceKernelGetProcessTime);
	RESOLVE(libkernel, sceKernelGetCurrentCpu);

	RESOLVE(libkernel, sysctl);
	RESOLVE(libkernel, sysctlbyname);
	RESOLVE(libkernel, sysarch);
	RESOLVE(libkernel, execve);

	RESOLVE(libkernel, pthread_self);
	RESOLVE(libkernel, pthread_setaffinity_np);

	RESOLVE(libkernel, sceKernelCreateEqueue);
	RESOLVE(libkernel, sceKernelDeleteEqueue);
	RESOLVE(libkernel, sceKernelAddUserEvent);
	RESOLVE(libkernel, sceKernelAddReadEvent);

	RESOLVE(libkernel, getuid);
	RESOLVE(libkernel, getgid);
	RESOLVE(libkernel, getpid);

	RESOLVE(libkernel, setuid);
	RESOLVE(libkernel, setgid);
	RESOLVE(libkernel, setreuid);
	RESOLVE(libkernel, setregid);
}
