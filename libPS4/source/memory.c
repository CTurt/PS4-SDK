#include "syscall.h"

#include "memory.h"

SYSCALL(mmap, 477);
SYSCALL(munmap, 73);
SYSCALL(mprotect, 74);
SYSCALL(msync, 65);
SYSCALL(mlock, 203);
SYSCALL(munlock, 204);

SYSCALL(getMemoryInfo, 547);
SYSCALL(getOtherMemoryInfo, 572);
