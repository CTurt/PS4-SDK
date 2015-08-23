#include "syscall.h"

#include "eventflag.h"

int createEventFlag(const char *name) {
	return syscall(538, name);
}

int destroyEventFlag(int eventFlag) {
	return syscall(539, eventFlag);
}
