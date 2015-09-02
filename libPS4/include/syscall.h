#pragma once

typedef unsigned long (*f)();
#define directSyscall (*(f *)0x9263ffff8)

unsigned long syscall(unsigned long n, ...);
