#include "syscall.h"

#include "file.h"

int getSandboxDirectory(char *destination, int *length) {
	return syscall(602, 0, destination, length);
}
