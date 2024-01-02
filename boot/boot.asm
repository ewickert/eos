.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

# mbheader
.section .multiboot.data, "aw"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 #16kb
stack_top:

.section .bss, "aw", @nobits
    .align 4096
boot_page_directory:
    .skip 4096
boot_page_table1:
    .skip 4096
.section .multiboot.text, "a"
.global _start
.type _start, @function
_start:
    #store the page table in edi
    movl $(boot_page_table1 - 0xC0000000), %edi
    #start at 0x0
    movl $0, %esi
    # Map 1023 pages, 1024th will be VGA
    movl $1023, %ecx
1:
    # we aren't at the kernel yet
    # cmpl $_kernel_start, %esi
    #jl 2f
    #we are past the kernel
    cmpl $(_kernel_end - 0xC0000000), %esi
    jge 3f
    
    # put current addr in edx
    movl %esi, %edx
    # |= 3, to set present and writable. Need to fix so .text and .rodata aren't
    # writable
    orl $0x003, %edx
    #store it in the page table
    movl %edx, (%edi)
2:
    // go forward 1 page in memory
    addl $4096, %esi
    // offset into page boot_page_table++
    addl $4, %edi
    loop 1b
3:
    # VGA is moving to 0xC03FF000
    # VGA is moved to boot_page_table1[1023]
    movl $(0x000B8000 | 0x003), boot_page_table1 - 0xC0000000 + 1023 * 4
    // boot_page_table1 mapped to 0 & 0xC03FFFFF (boot_page_directory[0] and boot_page_directory[768])
    movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
    movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4
    // recursive map boot_page_directory to boot_page_directory[1023]
    movl $(boot_page_directory - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 1023 * 4

    #set cr3 to boot_page_directory
    movl $(boot_page_directory - 0xC0000000), %ecx
    movl %ecx, %cr3

    #enable paging, turn on write-protect
    movl %cr0, %ecx
    orl $0x80010000, %ecx
    movl %ecx, %cr0

    #absolute jump to higher half
    lea 4f, %ecx
    jmp *%ecx
    
.section .text

4:
    #wipe out the identity mapping, boot_page_directory[0] is now free
    //movl $0, boot_page_directory + 0

    #TLB flush
    movl %cr3, %ecx
    movl %ecx, %cr3
    mov $stack_top, %esp
    sti
    # put multiboot onto the stack
    push %eax
    push %ebx
    call kernel_main
1:  hlt
    jmp 1b