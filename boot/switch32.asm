[bits 16]
switch_to_32:
    cli ; disable interrupts
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1 ; enable protected mode
    mov cr0, eax
    jmp CODE_SEG:init_32bit ; far jump

[bits 32]
init_32bit:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x9000
    mov esp, ebp
    
    call begin_32