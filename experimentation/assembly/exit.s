.text
.globl main

main:
	mov $60, %rax
	mov $42, %rdi
	syscall 
