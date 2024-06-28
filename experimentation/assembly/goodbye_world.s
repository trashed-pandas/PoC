	.file	"goodbye_world.c"
	.section	.rodata
.LC0:
	.string	"Goodbye world!"
	.text
.globl goodbye
	.type	goodbye, @function
goodbye:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
	.cfi_def_cfa_register 6
	movl	$.LC0, %edi
	call	puts
	leave
	ret
	.cfi_endproc
.LFE0:
	.size	goodbye, .-goodbye
	.section	.rodata
.LC1:
	.string	"Hello world!"
	.text
.globl main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
	.cfi_def_cfa_register 6
	movl	$.LC1, %edi
	call	puts
	movl	$0, %eax
	call	goodbye
	movl	$0, %eax
	leave
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
