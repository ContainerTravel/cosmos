extern main

global _start

[section .text]
[BITS 64]
_start:
	xor rax,rax
	mov rax,main
	call rax
	jmp $
	