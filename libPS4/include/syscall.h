#pragma once

#define asm __asm__

#define SYSCALL(name, number) asm(".intel_syntax noprefix\n" ".global " #name "\n" #name ":\n" "\tmovq rax, " #number "\n\tjmp rawSyscall\n")

typedef unsigned long (*f)();
#define directSyscall (*(f *)0x9263ffff8)

unsigned long syscall(unsigned long n, ...);
