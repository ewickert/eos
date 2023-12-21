#include "../drivers/vga.h"
#include "../cpu/isr.h"
#include "../drivers/keyboard.h"
void _start()
{
    const char init_msg[] = "Initializing os\n";
    clear_screen();
    print_string("Initializing os\n"); // good
    isr_install();
    print_string("Enable external interrupts\n");
    asm volatile("sti");
    print_string("init keyboard\n");
    init_keyboard();
    clear_screen();
}