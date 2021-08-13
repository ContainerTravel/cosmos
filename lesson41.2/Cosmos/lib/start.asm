extern main
extern exit
global _start
[section .start.text]
[BITS 64]
_start:
	xor rax, rax
	mov rax, main
	call rax
	mov rsi, rax
exit_loop:
	mov rax, exit
	call rax
	jmp exit_loop
	