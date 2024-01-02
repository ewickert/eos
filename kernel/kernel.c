#include "../drivers/vga.h"
#include "../drivers/com.h"
#include "../cpu/isr.h"
#include "../cpu/gdt.h"
#include "../cpu/idt.h"
#include "../cpu/clock.h"
#include "../drivers/keyboard.h"
#include "../lib/stdlib.h"
#include "../lib/string.h"
#include "../mem/paging.h"
#include "mmap.h"
extern uint32_t _kernel_end;
extern uint32_t _kernel_start;
void print_mmap(multiboot_info_t *);
void kernel_main(multiboot_info_t *mbd, uint32_t magic)
{

    kerr_named_dword("MMAP at", mbd);
    kcls();
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        kputl("Invalid MB");
    }
    multiboot_info_t foo = *mbd;
    //     set_cursor(0);
    print_mmap(mbd);
    idt_init();
    kerr_ln("IDT set");
    gdt_init();
    kerr_ln("GDT set");
    init_keyboard();
    init_timer(10);
    init_paging();
    kputl("Hello, world");
    // uint32_t *ptr = (uint32_t *)0xA0000000;
    // uint32_t do_page_fault = *ptr;
}
void print_mmap(multiboot_info_t *mbd)
{
    int i = 0;
    for (i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t))
    {
        multiboot_memory_map_t *mmmt =
            (multiboot_memory_map_t *)(mbd->mmap_addr + i);
        kputi(mmmt->addr);
        kputs("->");
        kputi(mmmt->len);
        kputs("\r\n");
    }
}