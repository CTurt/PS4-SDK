#pragma once

ssize_t read(int fd, void *buf, size_t nbyte);
int open(char *path, int flags, int mode);
int close(int fd);
int getdents(int fd, char *buf, size_t count);

int getSandboxDirectory(char *destination, int *length);
