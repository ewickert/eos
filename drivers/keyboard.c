#include <stdint.h>
#include "../drivers/com.h"
#include "../lib/stdlib.h"
#include "../cpu/isr.h"
#include "keyboard.h"
#include <stdbool.h>

#define BACKSPACE 0xee
#define LSHIFT_D 0x2a
#define LSHIFT_U 0xaa
#define CAPS_OFFSET 0x20

#define DOCAPS(ascii) (ascii - (caps * CAPS_OFFSET))
#define ISNLIN(scancode) (scancode > 1 && scancode < 14)

bool caps;

const char scancodes[] =
    {
        '?', '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '?',
        '?', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's',
        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v', 'b',
        'n', 'm', ',', '.', '/', '?', '?', '?', ' '};

const char alt_codes[] =
    {
        '?', '?', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+'};

void init_keyboard()
{
    kerr_ln("registered kboard");
    caps = false;
    register_interrupt_handler(33, keyboard_callback);
}
static void keyboard_callback(registers_t regs)
{
    uint8_t scancode = p_read_byte(0x60);
    char code[5];
    citoa(scancode, code, 16);
    kerr_ln(code);
    if (scancode == LSHIFT_D)
    {
        caps = true;
        return;
    }
    if (scancode == LSHIFT_U)
    {
        caps = false;
        return;
    }
    if (scancode > 57)
        return;

    if (scancode == BACKSPACE)
    {
        kputc(0x08);
        // print_backspace();
    }
    else if (ISNLIN(scancode) && caps)
    {
        char sym = alt_codes[scancode];
        char str[2] = {sym, '\0'};
        kputs(str);
    }
    else
    {
        char letter = scancodes[scancode];
        char str[2] = {DOCAPS(letter), '\0'};
        kputs(str);
    }
}
