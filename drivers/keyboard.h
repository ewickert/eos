#include <stdint.h>
#include "../cpu/isr.h"

void print_letter(uint8_t scancode);
void init_keyboard();
static void keyboard_callback(registers_t *regs);