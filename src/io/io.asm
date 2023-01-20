section .asm

global insb
global insw
global outb
global outw

insb:
    push ebp
    mov ebp, esp

    xor eax, eax        ; eax = 0
    mov edx, [ebp+8]    ; edx = dx
    in al, dx           ; al = in(dx)

    pop ebp
    ret

insw:
    push ebp
    mov ebp, esp

    xor eax, eax        ; eax = 0
    mov edx, [ebp+8]    ; edx = dx
    in ax, dx           ; ax = in(dx)

    pop ebp
    ret

outb:
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]   ; al = al
    mov edx, [ebp+8]    ; edx = dx
    out dx, al          ; out(dx, al)

    pop ebp
    ret

outw:
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]   ; ax = ax
    mov edx, [ebp+8]    ; edx = dx
    out dx, ax          ; out(dx, ax)

    pop ebp
    ret