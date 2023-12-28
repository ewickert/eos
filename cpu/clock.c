#include "clock.h"
#include "isr.h"
#include "../drivers/com.h"
#include "../drivers/io.h"
#include <stdint.h>

uint32_t tick = 0;
static void timer_callback(registers_t r)
{
    // tick++;
    //  kerr_named_dword("tick", tick);
}

void init_timer(uint32_t freq)
{
    register_interrupt_handler(IRQ0, &timer_callback);
    uint32_t divisor = 1193180;
    p_write_byte(0x43, 0x36);
    uint8_t l = (uint8_t)divisor & 0xff;
    uint8_t h = (uint8_t)(divisor >> 8) & 0xff;
    p_write_byte(0x40, l);
    p_write_byte(0x40, h);
}