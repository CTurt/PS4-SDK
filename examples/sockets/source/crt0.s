.intel_syntax noprefix
.text

.global _start
_start:
	mov		rax, 0x9263FFFF8
	mov		[rax], rcx
	jmp		_main
