#include "syscall.h"

#include "unknown.h"

int unknownResourceCreate(const char *name) {
	return syscall(557, name);
}

int unknownResourceDestroy(int unknownResource) {
	return syscall(558, unknownResource);
}

int unknownResource2Create(const char *name) {
	return syscall(574, name);
}

int unknownResource2Destroy(int unknownResource) {
	return syscall(575, unknownResource);
}