#include "syscall.h"

#include "unknown.h"

int unknownResourceCreate(const char *name) {
	return syscall(574, name);
}

int unknownResourceDestroy(int unknownResource) {
	return syscall(575, unknownResource);
}