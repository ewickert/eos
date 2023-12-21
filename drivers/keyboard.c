#include <stdint.h>
#include "../drivers/vga.h"
#include "../cpu/isr.h"
#include "keyboard.h"

#define BACKSPACE 0x0e
const char scancodes[] =
    {
        '?', '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '?',
        '?', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's',
        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v', 'b',
        'n', 'm', ',', '.', '/', '?', '?', '?', ' '};

void init_keyboard()
{
    register_interrupt_handler(33, keyboard_callback);
}
static void keyboard_callback(registers_t *regs)
{
    uint8_t scancode = p_read_byte(0x60);
    if (scancode > 57)
        return;
    if (scancode == BACKSPACE)
    {
        print_backspace();
    }
    else
    {
        char letter = scancodes[scancode];
        char str[2] = {letter, '\0'};
        print_string(str);
    }
}
