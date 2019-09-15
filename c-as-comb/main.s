

section .text
	global main
	extern add_print

main:
	call add_print
	add rsp, 8

	; return 0;
	mov rax,60
	mov rdi,0
	syscall
