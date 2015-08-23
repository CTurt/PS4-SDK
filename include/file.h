#pragma once

#include "types.h"

#define O_RDONLY  0x0000
#define O_WRONLY  0x0001
#define O_RDWR    0x0002
#define O_ACCMODE 0x0003

#define O_BINARY    0x0004
#define O_TEXT      0x0008
#define O_NOINHERIT 0x0080

#define O_CREAT    0x0100
#define O_EXCL     0x0200
#define O_NOCTTY   0x0400
#define O_TRUNC    0x0800
#define O_APPEND   0x1000
#define O_NONBLOCK 0x2000

#define DT_UNKNOWN 0
#define DT_FIFO    1
#define DT_CHR     2
#define DT_DIR     4
#define DT_BLK     6
#define DT_REG     8
#define DT_LNK     10
#define DT_SOCK    12
#define DT_WHT     14

struct stat {
	uint64_t d[9];
	off_t st_size; // 0x48
};

struct dirent {
	uint32_t d_fileno;
	uint16_t d_reclen;
	uint8_t d_type;
	uint8_t d_namlen;
	char d_name[255 + 1];
};

typedef uint16_t mode_t;


ssize_t read(int fd, void *buf, size_t nbyte);
ssize_t write(int fd, const void *buf, size_t count);
int open(const char *path, int flags, int mode);
int close(int fd);
int unlink(const char *pathname);
int stat(const char *path, struct stat *sb);
int rename(const char *oldpath, const char *newpath);
int mkdir(const char *pathname, mode_t mode);
int rmdir(const char *path);
int fstat(int fd, struct stat *sb);
int getdents(int fd, char *buf, size_t count);

int getSandboxDirectory(char *destination, int *length);
