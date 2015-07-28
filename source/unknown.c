#include "syscall.h"

#include "unknown.h"

int unknownResourceCreate(const char *name) {
	syscall(557, name);
}

int unknownResourceDestroy(int unknownResource) {
	syscall(558, unknownResource);
}

int unknownResource2Create(const char *name) {
	syscall(574, name);
}

int unknownResource2Destroy(int unknownResource) {
	syscall(575, unknownResource);
}