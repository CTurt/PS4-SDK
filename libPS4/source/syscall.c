__asm__(".intel_syntax noprefix\n\
.text");

__asm__(".global syscall\n\
syscall:\n\
	push	r15\n\
	movq    r15, 0x9263FFFF8\n\
	mov     r15, [r15]\n\
	xor     rax, rax\n\
	call    r15\n\
	pop		r15\n\
	ret");
