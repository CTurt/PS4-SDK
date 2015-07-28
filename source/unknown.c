#include "syscall.h"

#include "unknown.h"

int unknownResourceCreate(const char *name) {
	syscall(557, name);
}

int unknownResourceDestroy(int unknownResource) {
	syscall(558, unknownResource);
}
