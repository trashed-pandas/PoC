	.file	"b0f.c"
	.text
	.p2align 4,,15
.globl boom
	.type	boom, @function
boom:
.LFB12:
	.cfi_startproc
	subq	$136, %rsp
	.cfi_def_cfa_offset 144
	movq	%rdi, %rsi
	movq	%rsp, %rdi
	call	strcpy
	addq	$136, %rsp
	ret
	.cfi_endproc
.LFE12:
	.size	boom, .-boom
	.p2align 4,,15
.globl main
	.type	main, @function
main:
.LFB13:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	8(%rsi), %rdi
	call	boom
	xorl	%eax, %eax
	addq	$8, %rsp
	ret
	.cfi_endproc
.LFE13:
	.size	main, .-main
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
