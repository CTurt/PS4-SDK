#pragma once

#define asm __asm__

#define SYSCALL(name, number) asm(".intel_syntax"); asm(#name ":"); asm("\tmovq rax, " #number); asm("\tjmp rawSyscall")

typedef unsigned long (*f)();
#define directSyscall (*(f *)0x9263ffff8)

unsigned long syscall(unsigned long n, ...);
