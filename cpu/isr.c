#include "isr.h"
#include "../drivers/com.h"
#include "../drivers/io.h"
isr_t interrupt_handlers[256];

void isr_handler(registers_t *r)
{
    if (interrupt_handlers[r->int_no])
    {
        interrupt_handlers[r->int_no](r);
    }
    kerr_named_dword("int:", r->int_no);
    if (r->int_no == 0xd && r->err_code)
    {
        kerr_named_dword("Segfault", r->eip);
        while (1)
            ;
    }
}
void irq_handler(registers_t *r)
{

    if (interrupt_handlers[r->int_no] != 0)
    {
        interrupt_handlers[r->int_no](r);
    }
    else
    {
        kerr_named_dword("Unhandled irq", r->int_no - 0x20);
    }
    p_write_byte(0x20, 0x20);
    if (r->int_no < 40)
    {
        p_write_byte(0xA0, 0x20);
    }
}
void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}