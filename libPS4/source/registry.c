#include "syscall.h"

#include "registry.h"

int registryCommand(int command /*, ...*/) {
	return syscall(532, command);
}
