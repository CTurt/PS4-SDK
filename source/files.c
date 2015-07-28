#include "syscall.h"

#include "files.h"

int getSandboxDirectory(char *destination, int *length) {
	syscall(602, 0, destination, length);
}
