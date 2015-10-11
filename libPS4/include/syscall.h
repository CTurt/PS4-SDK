#pragma once

#define asm __asm__

//static inline unsigned long rSyscall() {
	__asm__(".intel_syntax noprefix\n\
rSyscall:\n\
	push	r15\n\
	movq    r15, 0x9263FFFF8\n\
	mov     r15, [r15]\n\
	call    r15\n\
	pop		r15\n\
	ret");
//}

#define SYSCALL(name, number) asm(".intel_syntax noprefix\n" ".global " #name "\n" #name ":\n" "\tmovq rax, " #number "\n\tcall rSyscall\n\tret\n")

typedef unsigned long (*f)();
#define directSyscall (*(f *)0x9263ffff8)

unsigned long syscall(unsigned long n, ...);
