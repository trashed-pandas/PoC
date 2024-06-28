global _start

section .text
_start: 

	mov rax, 1
	mov rdi, 1
	mov rsi, hw
	mov rdx, hw_len
	syscall

	mov rax, 60
	mov rdi, 0
	syscall

section .data
	hw:	db 'Hello world!', 0x0a
	hw_len: equ $-hw
