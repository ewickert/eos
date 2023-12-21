disk_load:
    pusha
    push dx
    ;mov ah, 0x0e ; tty mode
    ;mov al, 'D'
    ;int 0x10

    mov ah, 0x02 ; read mode
    mov al, dh   ; # sectors
    mov cl, 0x02 ; start from sector 2
    mov ch, 0x00 ; cylinder 0
    mov dh, 0x00 ; head 0
    ;dl set to drive # as arg
    ; es:bx buffer pointer set as input

    int 0x13 ; int for disk
    jc disk_error
    ;mov ah, 0x0e ; tty mode
    ;mov al, '2'
    ;int 0x10
    pop dx ;get back original sectors to read
    cmp al, dh ; make sure we read them
    jne sector_error
    popa
    ret

disk_error:
    ;mov ah, 0x0e ; tty mode
    ;mov al, 'E'
    ;int 0x10
    ;mov ah, 0x1
    ;mov al, 0
    ;mov dl, 0
    ;int 0x13
    jmp $
    
sector_error:
    mov ah, 0x0e ; tty mode
    mov al, 'S'
    int 0x10
    mov ah, 0x1
    mov al, 0
    mov dl, 0
    int 0x13
    jmp $