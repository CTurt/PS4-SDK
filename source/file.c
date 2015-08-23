#include "syscall.h"

#include "file.h"

ssize_t read(int fd, void *buf, size_t nbyte) {
	return syscall(3, fd, buf, nbyte);
}

int open(char *path, int flags, int mode) {
	return syscall(5, path, flags, mode);
}

int close(int fd) {
	return syscall(6, fd);
}

int getdents(int fd, char *buf, size_t count) {
	return syscall(272, fd, buf, count);
}

int getSandboxDirectory(char *destination, int *length) {
	return syscall(602, 0, destination, length);
}
