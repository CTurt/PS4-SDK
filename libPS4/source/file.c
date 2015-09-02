#include "syscall.h"

#include "file.h"

ssize_t read(int fd, void *buf, size_t nbyte)
{
	return syscall(3, fd, buf, nbyte);
}

ssize_t write(int fd, const void *buf, size_t count)
{
	return syscall(4, fd, buf, count);
}

int open(const char *path, int flags, int mode)
{
	return syscall(5, path, flags, mode);
}

int close(int fd)
{
	return syscall(6, fd);
}

int unlink(const char *pathname)
{
	return syscall(10, pathname);
}

int rename(const char *oldpath, const char *newpath)
{
	return syscall(128, oldpath, newpath);
}

int mkdir(const char *pathname, mode_t mode)
{
	return syscall(136, pathname, mode);
}

int rmdir(const char *path)
{
	return syscall(137, path);
}

int stat(const char *path, struct stat *sb)
{
	return syscall(188, path, sb);
}

int fstat(int fd, struct stat *sb)
{
	return syscall(189, fd, sb);
}

int getdents(int fd, char *buf, size_t count)
{
	return syscall(272, fd, buf, count);
}

int getSandboxDirectory(char *destination, int *length)
{
	return syscall(602, 0, destination, length);
}
