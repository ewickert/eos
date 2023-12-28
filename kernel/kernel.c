#include "../drivers/vga.h"
#include "../drivers/com.h"
#include "../cpu/isr.h"
#include "../cpu/gdt.h"
#include "../cpu/idt.h"
#include "../cpu/clock.h"
#include "../drivers/keyboard.h"
#include "../lib/stdlib.h"
#include "../lib/string.h"
extern uint32_t endkernel;
extern uint32_t code;

void kernel_main()
{

    kcls();
    kputl("Hello, world");
    // set_cursor(0);
    gdt_init();
    kerr_ln("GDT set");
    idt_init();
    kerr_ln("IDT set");
    init_keyboard();
    init_timer(1000);
}