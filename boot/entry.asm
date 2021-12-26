; code will be loaded here
org 0x07c00

; real-mode code
; load next stage code
; enter protected mode

cpu 386
bits 16

; BIOS sets boot drive in 'dl'; store for later use
mov [BOOT_DRIVE], dl
jmp start

%include "bios_disk.asm"

start:
    ; setup segment registers
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; setup stack
    mov bp, STACK
    mov sp, STACK

    call load_next_stage
    ; enter protected mode on success
    cmp al, 0
    je enter_protected_mode

    ; error loading data
    jmp $


load_next_stage:
    mov bx, NEXT_STAGE_LOAD_ADDRESS ; bx -> destination
    mov dh, 20             ; dh -> num sectors
    mov dl, [BOOT_DRIVE]  ; dl -> disk
    call disk_load
    ret


enter_protected_mode:
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp CODE_SEGMENT:protected_mode_start

; protected-mode code
; setup protected mode
; call the next stage

bits 32

protected_mode_start:
    mov ax, DATA_SEGMENT
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; setup stack
    mov ebp, STACK
    mov esp, STACK

    call enter_next_stage
    jmp $

enter_next_stage:
    call NEXT_STAGE_LOAD_ADDRESS
	ret

; some memory reserved for use by the program

; define data for the GDT
gdt_start:
    ; null descriptor
    dq 0x0


; code segment descriptor
gdt_code:
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10011010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31

; data segment descriptor
gdt_data:
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10010010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31


gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; size (16 bit)
    dd gdt_start ; address (32 bit)

; boot drive variable
BOOT_DRIVE db 0

CODE_SEGMENT equ gdt_code - gdt_start
DATA_SEGMENT equ gdt_data - gdt_start
STACK equ 0x07c00
NEXT_STAGE_LOAD_ADDRESS equ 0x1000

; padding and magic to identify this as bootable
times 510 - ($-$$) db 0
dw 0xaa55
