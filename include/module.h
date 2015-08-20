#pragma once

#include <stdlib.h>

//#define RESOLVE(module, name) getFunctionAddressByName(module, #name, &name)
#define RESOLVE(module, name) do { char n[] = #name; getFunctionAddressByName(module, n, &name); } while(0)

struct moduleInfo {
	size_t size; // 0x0
	char name[32]; // 0x8
	char unknown[0xe0]; // 0x28
	void *codeBase; // 0x108
	unsigned int codeSize; // 0x110
	void *dataBase; // 0x118
	unsigned int dataSize; // 0x120
};

int getFunctionAddressByName(int loadedModuleID, char *name, void *destination);
int getLoadedModules(int *destination, int max, int *count);
int getModuleInfo(int loadedModuleID, struct moduleInfo *destination);
int loadModule(char *name, int *idDestination);
