bits 16
disk_load:
    ; in
    ; bx = destination address
    ; dh = number of sectors
    ; dl = disk
    ; out
    ; al = error code: 0 = success
    push dx

    mov ah, 0x02 ; read mode
    mov al, dh   ; read dh number of sectors
    mov cl, 0x02 ; start from sector 2
                 ; (as sector 1 is our boot sector)
    mov ch, 0x00 ; cylinder 0
    mov dh, 0x00 ; head 0

    ; dl = drive number is set as input to disk_load
    ; es:bx = buffer pointer is set as input as well

    int 0x13      ; BIOS interrupt
    jc disk_load_disk_error ; check carry bit for error

    pop dx     ; get back original number of sectors to read
    cmp al, dh ; BIOS sets 'al' to the # of sectors actually read
               ; compare it to 'dh' and error out if they are !=
    jne disk_load_sectors_error
    mov al, 0
disk_load_end:
    ret
disk_load_disk_error:
    mov al, 1
    jmp disk_load_end
disk_load_sectors_error:
    mov al, 2
    jmp disk_load_end
