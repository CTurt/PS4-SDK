.intel_syntax noprefix
.text

.global syscall
syscall:
	push	r15
	mov     r15, 0x9263FFFF8
	mov     r15, [r15]
	xor     rax, rax
	call    r15
	pop		r15
	ret
