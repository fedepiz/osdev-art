;This file contains helper functions that are coded in assembler, either because
;this is needed to be so or because of convenience.
section .text

global asm_add
asm_add:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov ecx, [ebp+12]
	add eax, ecx
	pop ebp
	ret

global jump_to_address
jump_to_address:
;;The address is the first argument, let's pop it into eax
	push ebp
	mov ebp, esp
	mov eax, [esp+8]
	pop ebp
	push eax
	ret

global halt
halt:
	hlt
	ret

global get_eflags
get_eflags:
	pushf
	mov eax, [esp]
	popf
	ret