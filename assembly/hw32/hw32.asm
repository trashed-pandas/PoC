

global _start


section .text
_start:

	; write(STDOUT_FILENO, msg, strlen(msg));
	mov	eax, 4
	mov ebx, 1
	mov	ecx, msg
	mov	edx, msg.len
	int	0x80

	; exit(0);
	mov	eax, 1
	xor	ebx, ebx
	int 0x80


section .data

msg: db	"Hello world!", 0x0a
.len: equ	$ - msg
