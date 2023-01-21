section .asm

extern int21h_handler
extern no_interrupt_handler

global int21h
global no_interrupt
global idt_load
global enable_interrupts
global disable_interrupts


idt_load:
    push ebp 
    mov ebp, esp
    
    mov ebx, [ebp+8]
    lidt [ebx]

    pop ebp
    ret

enable_interrupts:
    sti     ; enable interrupts
    ret

disable_interrupts:
    cli     ; disable interrupts
    ret

int21h:
    cli     ; disable interrupts
    pushad  ; save registers
    
    call int21h_handler ; call the handler
    
    popad   ; restore registers
    sti     ; enable interrupts    
    iret    ; return from interrupt

no_interrupt:
    cli     ; disable interrupts
    pushad  ; save registers
    
    call no_interrupt_handler ; call the handler
    
    popad   ; restore registers
    sti     ; enable interrupts    
    iret    ; return from interrupt