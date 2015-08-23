#pragma once

#include "types.h"

struct stat {
	uint64_t d[9];
	off_t st_size; // 0x48
};

ssize_t read(int fd, void *buf, size_t nbyte);
int open(const char *path, int flags, int mode);
int close(int fd);
int fstat(int fd, struct stat *sb);
int getdents(int fd, char *buf, size_t count);

int getSandboxDirectory(char *destination, int *length);
