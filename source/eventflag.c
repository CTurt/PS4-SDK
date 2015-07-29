#include "syscall.h"

#include "eventflag.h"

int createEventFlag(const char *name) {
	return syscall(538, name);
}

int destroyEventFlag(int unknownResource) {
	return syscall(539, unknownResource);
}
