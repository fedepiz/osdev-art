;This file contains helper functions that are coded in assembler, either because
;this is needed to be so or because of convenience.
section .text

global read_cr2

read_cr2:
   mov eax, cr2
   ret

global outb             ; make the label outb visible outside this file

; outb - send a byte to an I/O port
; stack: [esp + 8] the data byte
;        [esp + 4] the I/O port
;        [esp    ] return address
outb:
	  mov al, [esp + 8]    ; move the data to be sent into the al register
    mov dx, [esp + 4]    ; move the address of the I/O port into the dx register
	  out dx, al           ; send the data to the I/O port
    ret

global inb

; inb - returns a byte from the given I/O port
; stack: [esp + 4] The address of the I/O port
;        [esp    ] The return address
inb:
    mov dx, [esp + 4]       ; move the address of the I/O port to the dx register
    in  al, dx              ; read a byte from the I/O port and store it in the al register
    ret                     ; return the read byte

		; This will set up our new segment registers. We need to do
		; something special in order to set CS. We do what is called a
		; far jump. A jump that includes a segment as well as an offset.
		; This is declared in C as 'extern void gdt_flush();'
global gdt_flush     ; Allows the C code to link to this
extern gp            ; Says that '_gp' is in another file
gdt_flush:
		lgdt [gp]        ; Load the GDT with our '_gp' which is a special pointer
	  mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
	  mov ds, ax
	  mov es, ax
	  mov fs, ax
	  mov gs, ax
	  mov ss, ax
	  jmp 0x08:flush2   ; 0x08 is the offset to our code segment: Far jump!
flush2:
		ret               ; Returns back to the C code!


; Loads the IDT defined in '_idtp' into the processor.
; This is declared in C as 'extern void idt_load();'
global idt_load
extern idtp
idt_load:
		lidt [idtp]
		ret



; In just a few pages in this tutorial, we will add our Interrupt
; Service Routines (ISRs) right here!
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

%macro no_error_code_interrupt_handler 1
isr%1:
		cli
		push byte 0
		push byte %1        ; Note that we DON''T push a value on the stack in this one!
		                   ; It pushes one already! Use this type of stub for exceptions
		                   ; that pop error codes!
		jmp isr_common_stub
		;  0: Divide By Zero Exception
%endmacro

%macro error_code_interrupt_handler 1
isr%1:
	cli
	;push byte 0
	push byte %1
	jmp isr_common_stub
%endmacro

no_error_code_interrupt_handler 0
no_error_code_interrupt_handler 1
no_error_code_interrupt_handler 2
no_error_code_interrupt_handler 3
no_error_code_interrupt_handler 4
no_error_code_interrupt_handler 5
no_error_code_interrupt_handler 6
no_error_code_interrupt_handler 7
error_code_interrupt_handler 8
no_error_code_interrupt_handler 9
error_code_interrupt_handler 10
error_code_interrupt_handler 11
error_code_interrupt_handler 12
error_code_interrupt_handler 13
error_code_interrupt_handler 14
no_error_code_interrupt_handler 15
no_error_code_interrupt_handler 16
no_error_code_interrupt_handler 17
no_error_code_interrupt_handler 18
no_error_code_interrupt_handler 19
no_error_code_interrupt_handler 20
no_error_code_interrupt_handler 21
no_error_code_interrupt_handler 22
no_error_code_interrupt_handler 23
no_error_code_interrupt_handler 24
no_error_code_interrupt_handler 25
no_error_code_interrupt_handler 26
no_error_code_interrupt_handler 27
no_error_code_interrupt_handler 28
no_error_code_interrupt_handler 29
no_error_code_interrupt_handler 30
no_error_code_interrupt_handler 31

		; We call a C function in here. We need to let the assembler know
		; that '_fault_handler' exists in another file
extern fault_handler

	; This is our common ISR stub. It saves the processor state, sets
	; up for kernel mode segments, calls the C-level fault handler,
	; and finally restores the stack frame.
isr_common_stub:
	  pusha
	  push ds
	  push es
	  push fs
	  push gs
	  mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
	  mov ds, ax
	  mov es, ax
	  mov fs, ax
	  mov gs, ax
	  mov eax, esp   ; Push us the stack
	  push eax
	  mov eax, fault_handler
	  call eax       ; A special call, preserves the 'eip' register
	  pop eax
	  pop gs
	  pop fs
	  pop es
	  pop ds
    popa
	  add esp, 8     ; Cleans up the pushed error code and pushed ISR number
		iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

;IRQ stuff
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

;IRQS 0-15 are mapped into interrupts 32-47

%macro irq_interrupt_handler 1
irq%1:
	cli
	push byte 0
	push byte (32 + %1)
	jmp irq_common_stub
%endmacro

irq_interrupt_handler 0
irq_interrupt_handler 1
irq_interrupt_handler 2
irq_interrupt_handler 3
irq_interrupt_handler 4
irq_interrupt_handler 5
irq_interrupt_handler 6
irq_interrupt_handler 7
irq_interrupt_handler 8
irq_interrupt_handler 9
irq_interrupt_handler 10
irq_interrupt_handler 11
irq_interrupt_handler 12
irq_interrupt_handler 13
irq_interrupt_handler 14
irq_interrupt_handler 15

extern irq_handler
; This is a stub that we have created for IRQ based ISRs. This calls
; '_irq_handler' in our C code. We need to create this in an 'arch.cpp'
irq_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, irq_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

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
