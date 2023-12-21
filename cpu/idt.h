#include <stdint.h>
#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)((address) >> 16 & 0xFFFF)
typedef struct
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t always0;
    uint8_t flags;
    uint16_t offset_high;
} __attribute__((packed)) idt_gate_t;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;

void set_idt_gate(int n, uint32_t handler);
void load_idt();