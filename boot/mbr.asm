[bits 16]
[org 0x7c00]

KERNEL_OFFSET equ 0x1000 ; Load kernel here
mov [BOOT_DRIVE], dl ; Store boot drive in d(l)

mov bp, 0x9000
mov sp, bp ; stack
mov dh, 31
call load_kernel
call switch_to_32

jmp $

%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/switch32.asm"

[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET ; bx -> dst
    mov dh, 0x12; dh -> # sectors
    mov dl, [BOOT_DRIVE] ; dl -> disk#
    call disk_load
    ret

[bits 32]
begin_32:
    call KERNEL_OFFSET ; go to kernel
    jmp $ ; uh oh

BOOT_DRIVE db 0
times 510 - ($-$$) db 0
dw 0xaa55