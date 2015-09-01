#include "syscall.h"
#include "kernel.h"

#include "module.h"

int (*sceSysmoduleLoadModule)(int id);

int getFunctionAddressByName(int loadedModuleID, char *name, void *destination) {
	return syscall(591, loadedModuleID, name, destination);
}

int getLoadedModules(int *destination, int max, int *count) {
	return syscall(592, destination, max, count);
}

int getModuleInfo(int loadedModuleID, struct moduleInfo *destination) {
	return syscall(593, loadedModuleID, destination);
}

int loadModule(const char *name, int *idDestination) {
	return syscall(594, name, 0, idDestination, 0);
}

int unloadModule(int id) {
	return syscall(595, id, 0, 0);
}

void initModule(void) {
	int libModule = sceKernelLoadStartModule("libSceSysmodule.sprx", 0, NULL, 0, 0, 0);
	
	// Just use sceKernelLoadStartModule instead
	RESOLVE(libModule, sceSysmoduleLoadModule);
}
