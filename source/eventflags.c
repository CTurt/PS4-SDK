#include "syscall.h"

#include "eventflags.h"

int createEventFlag(const char *name) {
	syscall(538, name);
}

int destroyEventFlag(int unknownResource) {
	syscall(539, unknownResource);
}
