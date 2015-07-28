#include "syscall.h"

#include "files.h"

int getSandboxDirectory(char *destination, int *length) {
	return syscall(602, 0, destination, length);
}
