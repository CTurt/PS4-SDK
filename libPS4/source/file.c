#include "syscall.h"

#include "file.h"

SYSCALL(read, 3);
SYSCALL(write, 4);
SYSCALL(open, 5);
SYSCALL(close, 5);
SYSCALL(unlink, 10);
SYSCALL(rename, 128);
SYSCALL(mkdir, 136);
SYSCALL(rmdir, 137);
SYSCALL(stat, 188);
SYSCALL(fstat, 189);
SYSCALL(getdents, 272);
SYSCALL(getSandboxDirectory, 602);
