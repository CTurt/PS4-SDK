#include "syscall.h"
#include "kernel.h"

#include "module.h"

int (*sceSysmoduleLoadModule)(int id);

SYSCALL(getFunctionAddressByName, 591);
SYSCALL(getLoadedModules, 592);
SYSCALL(getModuleInfo, 593);
SYSCALL(loadModule, 594);
SYSCALL(unloadModule, 595);

void initModule(void) {
	int libModule = sceKernelLoadStartModule("libSceSysmodule.sprx", 0, NULL, 0, 0, 0);
	
	// Just use sceKernelLoadStartModule instead
	RESOLVE(libModule, sceSysmoduleLoadModule);
}
