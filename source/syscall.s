.intel_syntax noprefix
.text

.global syscall
syscall:
	push	r14
	push	r15
	mov     r14, 0x9263FFFF8
	mov     r15, [r14]
	xor     rax, rax
	call    r15
	pop		r15
	pop		r14
	ret
