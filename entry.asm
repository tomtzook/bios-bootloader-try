; code will be loaded here
org 0x07c00

bits 16

section .text

main:
    jmp start
    nop

start:
    cli
    ; setup segment registers
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; setup stack
    mov bp, stack
    mov sp, stack

    sti
    ; call _main
    ret


section .data

; reserve space for stack
; stack grows downwards
stack_end:
    resb 4096
stack:
